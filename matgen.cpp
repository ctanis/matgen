#include <cstdio>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <time.h>



int main(int argc, char *argv[])
{
    int dim = atoi(argv[1]);
    double nnz_row = atoi(argv[2]);
    double var = atof(argv[3]);

    srand(time(NULL) ^ getpid());

    
    std::vector<int>    ia;
    std::vector<int>    ja;
    std::vector<double> data;

    ia.resize(dim+1);
    int res_size = (int)(1.0*nnz_row*dim*(1.0+var));

    fprintf(stderr, "reserving %d\n", res_size);

    ja.reserve(res_size);
    data.reserve(ja.size());

    for (int i=0; i<dim; i++)
    {
        if (i % (dim/10) == 0)
        {
            fprintf(stderr, "%d/%d - %d\n", i, dim, (int)ja.size());
        }

        ia[i] = ja.size()+1;

        // count of items in this row
        //double rate = var*(1.0*rand()/RAND_MAX-0.5)+density;

        double rate = 1.0*nnz_row / dim+var*nnz_row*(1.0*rand()/RAND_MAX-0.5)/dim;


        if (rate < 0) rate = 0;
        if (rate > 1) rate = 1;

        int thresh = (int)(RAND_MAX*rate);
        // count * random columns


        for (int j=0; j<dim; j++)
        {
            if (rand() < thresh)
            {
                ja.push_back(j);
                data.push_back(1000.0*rand()/RAND_MAX-50.0);
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
