
#include <scai/tracing.hpp>
#include <scai/common/Walltime.hpp>

#include <cstdio>

void subA( int& X )
{
    SCAI_REGION( "A" )
    ++X;
}

void subB( int& X )
{
    SCAI_REGION( "B" )
    X++;
}

int main()
{
    double time = scai::common::Walltime::get();
    int X = 0;
    SCAI_REGION( "main" )

    for ( int i = 0; i < 10000; ++i )
    {
        for ( int j = 0; j < 30; ++ j )
        {
            subA( X );
        }

        for ( int j = 0; j < 20; ++ j )
        {
            subB( X );
        }
    }

    time = scai::common::Walltime::get() - time;
    printf( "X = %d, number of calls, time = %f s\n", X, time );
}
