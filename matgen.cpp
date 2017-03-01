#include <cstdio>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <time.h>


#define MYRAND() rand()

// int g_seed;
// inline int fastrand() { 
//   g_seed = (214013*g_seed+2531011); 
//   return (g_seed>>16)&0x7FFF; 
// } 



int main(int argc, char *argv[])
{
    int dim = atoi(argv[1]);
    double nnz_row = atoi(argv[2]);
    double var = atof(argv[3]);

    srand(time(NULL) ^ getpid());
    //g_seed=time(NULL) ^ getpid();
    
    std::vector<int>    ia;
    std::vector<int>    ja;
    std::vector<double> data;

    ia.resize(dim+1);
    int res_size = (int)(1.0*nnz_row*dim*(1.0+var));

    fprintf(stderr, "reserving %d\n", res_size);

    ja.reserve(res_size);
    data.reserve(res_size);

    for (int i=0; i<dim; i++)
    {
        if (i % (dim/10) == 0)
        {
            fprintf(stderr, "%d/%d - %d\n", i, dim, (int)ja.size());
        }

        ia[i] = ja.size()+1;

        // calculate a likelihood that a given column has a non-zero, so that
        // we get near (1 +/- var)*nnz_row
        // double chance = 0.5(1.0+var*rand()/RAND_MAX)*nnz

        double chance = (1 - var + 2.0*var*MYRAND()/RAND_MAX)*nnz_row / dim;

        if (chance < 0) chance = 0;
        if (chance > 1) chance = 1;

        int thresh = (int)(RAND_MAX*chance);
        // count * random columns


        for (int j=0; j<dim; j++)
        {
            if (j == i || MYRAND() < thresh) // force a diagonal
            {
                ja.push_back(j);

                // weird scale factor
                data.push_back(1000.0*MYRAND()/RAND_MAX-500.0);
            }
        }
    }
    ia[dim]=ja.size()+1;

    printf("Dimension\n%d\nnnz\n%d\nIA\n", dim, (int)ja.size());

    for (unsigned int i=0; i<ia.size(); i++)
    {
        printf("%d\n", ia[i]);
    }

    printf("JA\n");
    for (unsigned int i=0; i<ja.size(); i++)
    {
        printf("%d\n", ja[i]);
    }

    printf("A\n");
    for (unsigned int i=0; i<data.size(); i++)
    {
        printf("%lf\n", data[i]);
    }
    

    return 0;
}
