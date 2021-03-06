/**
 * @file MetisDistribution.cpp
 *
 * @license
 * Copyright (c) 2013
 * Fraunhofer Institute for Algorithms and Scientific Computing SCAI
 * for Fraunhofer-Gesellschaft
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * @endlicense
 *
 * @brief MetisDistribution.cpp
 * @author Lauretta Schubert
 * @date 01.07.2013
 * @since 1.1.0
 */

// hpp
#include <scai/lama/distribution/MetisDistribution.hpp>

// local library
#include <scai/lama/distribution/NoDistribution.hpp>
#include <scai/lama/matrix/CSRSparseMatrix.hpp>

// internal scai libraries
#include <scai/tracing.hpp>

extern "C"
{
#include <metis.h>
}

namespace scai
{

namespace lama
{

SCAI_LOG_DEF_LOGGER( MetisDistribution::logger, "Distribution.MetisDistribution" )

#define MASTER 0

MetisDistribution::MetisDistribution( const CommunicatorPtr comm, const Matrix& matrix, std::vector<float>& weights )

    : GeneralDistribution( matrix.getNumRows(), comm )
{
    computeIt( comm, matrix, weights );
}

MetisDistribution::MetisDistribution( const CommunicatorPtr comm, const Matrix& matrix, float weight )

    : GeneralDistribution( matrix.getNumRows(), comm )
{
    SCAI_LOG_INFO( logger, "construct Metis distribution, weight at " << *comm << ": " << weight )

    // collect weights from all processors

    PartitionId numPartitions = comm->getSize();

    std::vector<float> weights( numPartitions );

    SCAI_LOG_INFO( logger, "#weights = " << weights.size() )

    comm->gather( &weights[0], 1, MASTER, &weight );
    comm->bcast( &weights[0], numPartitions, MASTER );

    // norm weights so that sum gives exactly 1.0

    normWeights( weights );

    SCAI_LOG_INFO( logger, "#weights = " << weights.size() )

    for( size_t i = 0; i < weights.size(); ++i )
    {
        SCAI_LOG_INFO( logger, "weight[" << i << "] = " << weights[i] )
    }

    computeIt( comm, matrix, weights );
}

void MetisDistribution::normWeights( std::vector<float>& weights )
{
    const size_t numPartitions = weights.size();

    // now each partition norms it

    float sum = 0;

    for( size_t i = 0; i < numPartitions; ++i )
    {
        sum += weights[i];
    }

    float sumNorm = 0.0f;

    for( size_t i = 0; i < numPartitions - 1; ++i )
    {
        weights[i] /= sum;
        sumNorm += weights[i];
    }

    weights[numPartitions - 1] = 1.0f - sumNorm;
}

void MetisDistribution::computeIt( const CommunicatorPtr comm, const Matrix& matrix, std::vector<float>& weights )
{
    // TODO check only for replicated matrix

    const _MatrixStorage& localMatrix = matrix.getLocalStorage();

    IndexType size = comm->getSize();
    IndexType myRank = comm->getRank();
    IndexType totalRows = matrix.getNumRows();

    if( myRank == MASTER )
    {
        // MASTER must have all values of the matrix to build the graph

        SCAI_ASSERT_EQUAL_ERROR( totalRows, localMatrix.getNumRows() )
        SCAI_ASSERT_EQUAL_ERROR( matrix.getNumColumns(), localMatrix.getNumColumns() )
    }

    std::vector<IndexType> numRowsPerOwner;
    std::vector<IndexType> distIA;
    std::vector<IndexType> rows;

    if( myRank == MASTER )
    {
        // test weights (tpwgts)
        // note: no weight can be zero for metis call
        // so reduce number of partitions and map partition index to the right process
        std::vector<real_t> tpwgts( size );
        std::vector<IndexType> mapping( size );

        IndexType count = 0;
        checkAndMapWeights( tpwgts, mapping, count, weights, size );

        // set size only for MASTER
        numRowsPerOwner.resize( size );
        distIA.resize( size + 1 );
        rows.resize( totalRows );

        if( count > 1 )
        {
            IndexType minConstraint = 0;
            std::vector<IndexType> partition( totalRows );
            callPartitioning( partition, minConstraint, count, tpwgts, comm, matrix );

            IndexType offsetCounter[size];

            // init
            for( IndexType i = 0; i < size; i++ )
            {
                numRowsPerOwner[i] = 0;
                offsetCounter[i] = 0;
            }

            //count rows per owner
            for( IndexType i = 0; i < totalRows; i++ )
            {
                ++numRowsPerOwner[mapping[partition[i]]];
            }

            // build "ia" array (running sums) for rows per owner
            distIA[0] = 0;

            for( IndexType i = 1; i < size + 1; i++ )
            {
                distIA[i] = distIA[i - 1] + numRowsPerOwner[i - 1];
            }

            // sort rows after owner
            for( IndexType i = 0; i < totalRows; i++ )
            {
                IndexType index = mapping[partition[i]];
                rows[distIA[index] + offsetCounter[index]] = i;
                ++offsetCounter[index];
            }
        }
        else
        {
            SCAI_LOG_WARN( logger,
                           "MetisDistribution called with 1 processor/1 weight, which is the same as NoDistribution." )

            for( IndexType i = 0; i < size; i++ )
            {
                numRowsPerOwner[i] = 0;
            }

            numRowsPerOwner[mapping[0]] = totalRows;

            for( IndexType i = 0; i < totalRows; i++ )
            {
                rows[i] = i;
            }
        }
    }

    // scatter local rows
    int numMyRows = 0;
    comm->scatter( &numMyRows, 1, MASTER, &numRowsPerOwner[0] );
    mLocal2Global.resize( numMyRows );
    comm->scatterV( &mLocal2Global[0], numMyRows, MASTER, &rows[0], &numRowsPerOwner[0] );

    std::vector<IndexType>::const_iterator end = mLocal2Global.end();
    std::vector<IndexType>::const_iterator begin = mLocal2Global.begin();

    for( std::vector<IndexType>::const_iterator it = begin; it != end; ++it )
    {
        IndexType i = static_cast<IndexType>( std::distance( begin, it ) );
        SCAI_ASSERT( 0 <= *it && *it < mGlobalSize,
                     *it << " is illegal index for general distribution of size " << mGlobalSize )
        mGlobal2Local[ *it] = i;
    }
}

MetisDistribution::~MetisDistribution()
{
    SCAI_LOG_INFO( logger, "~MetisDistribution" )
}

void MetisDistribution::writeAt( std::ostream& stream ) const
{
    // write identification of this object

    stream << "MetisDistribution( size = " << mLocal2Global.size() << " of " << mGlobalSize << ", comm = "
           << *mCommunicator << " )";
}

template<typename WeightType>
void MetisDistribution::callPartitioning(
    std::vector<IndexType>& partition,
    IndexType& minConstraint,
    IndexType& parts,
    std::vector<WeightType>& tpwgts,
    const CommunicatorPtr comm,
    const Matrix& matrix ) const
{
    SCAI_REGION( "METIScall" )

    IndexType totalRows = matrix.getNumRows();

    std::vector<IndexType> csrXadj( totalRows + 1 ); // #rows + 1
    std::vector<IndexType> csrAdjncy( matrix.getLocalNumValues() - totalRows ); // #values - #diagonalelements(rows)
    std::vector<IndexType> csrVwgt( totalRows );

    matrix.getLocalStorage().buildCSRGraph( &csrXadj[0], &csrAdjncy[0], &csrVwgt[0], comm );

    IndexType ncon = 1;
    IndexType options[METIS_NOPTIONS];
    METIS_SetDefaultOptions( options );
    options[METIS_OPTION_PTYPE] = METIS_PTYPE_RB;

    // use only with PartGraphKway
    // options[ METIS_OPTION_OBJTYPE ] = METIS_OBJTYPE_VOL;//METIS_OBJTYPE_CUT
    // options[ METIS_OPTION_DBGLVL ] = METIS_DBG_TIME;
    // recursive bisection
    METIS_PartGraphRecursive( &totalRows, &ncon, &csrXadj[0], &csrAdjncy[0], &csrVwgt[0], NULL, NULL, &parts,
                              &tpwgts[0], NULL, options, &minConstraint, &partition[0] );

    // multilevel k-way partitioning (used in ParMetis)
    // METIS_PartGraphKway( &totalRows, &ncon, &csrXadj[0], &csrAdjncy[0], &csrVwgt[0], NULL,
    // NULL, &parts, &tpwgts[0], NULL, options, &minConstraint, &partition[0] );
}

template<typename WeightType>
void MetisDistribution::checkAndMapWeights(
    std::vector<WeightType>& tpwgts,
    std::vector<IndexType>& mapping,
    IndexType& count,
    std::vector<float>& weights,
    IndexType size ) const
{
    for( IndexType i = 0; i < size; ++i )
    {
        if( weights[i] != 0 )
        {
            mapping[count] = i;
            tpwgts[count] = weights[i];
            ++count;
        }
    }
}

/* ---------------------------------------------------------------------------------*
 *   static create methods ( required for registration in distribution factory )    *
 * ---------------------------------------------------------------------------------*/

MetisDistribution* MetisDistribution::create(
    const CommunicatorPtr commPtr,
    const IndexType globalSize,
    const float weight )
{
    // create an empty CSR sparse matrix
    CSRSparseMatrix<float> matrix;
    DistributionPtr dist( new NoDistribution( globalSize ) );
    matrix.setIdentity( dist );
    return new MetisDistribution( commPtr, matrix, weight );
}

MetisDistribution* MetisDistribution::create( const CommunicatorPtr commPtr, const Matrix& matrix, const float weight )
{
    return new MetisDistribution( commPtr, matrix, weight );
}

/* ---------------------------------------------------------------------------------*/

bool MetisDistribution::initialized = Distribution::registerCreator<MetisDistribution>( "METIS" );

} /* end namespace lama */

} /* end namespace scai */
