#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include <proj.h>
#include <stdio.h>
#include <vector>

int main()
{
    PJ_CONTEXT *C;
    PJ *        P;
    PJ *        P_for_GIS;
    PJ_COORD    a, b;

    /* or you may set C=PJ_DEFAULT_CTX if you are sure you will     */
    /* use PJ objects from only one thread                          */
    C = proj_context_create();

    P = proj_create_crs_to_crs(C,
                               "+proj=tmerc +lat_0=0 +lon_0=116.5 +k=1 +x_0=500000 +y_0=0 +ellps=GRS80 +units=m +no_defs",
                               "EPSG:4326", /* or EPSG:32632 */
                               NULL);

    if (0 == P)
    {
        fprintf(stderr, "Oops\n");
        return 1;
    }

    /* This will ensure that the order of coordinates for the input CRS */
    /* will be longitude, latitude, whereas EPSG:4326 mandates latitude, */
    /* longitude */
    P_for_GIS = proj_normalize_for_visualization(C, P);
    if (0 == P_for_GIS)
    {
        fprintf(stderr, "Oops\n");
        return 1;
    }
    proj_destroy(P);
    P = P_for_GIS;

    /* a coordinate union representing Copenhagen: 55d N, 12d E    */
    /* Given that we have used proj_normalize_for_visualization(), the order of
    /* coordinates is longitude, latitude, and values are expressed in degrees. */
    //a = proj_coord(4057763.283695, 533873.961202, 42.050, 0);
    std::vector<PJ_COORD> values;
    printf("part1\n");
    values.push_back(proj_coord(530712.7142480, 4045555.0458590, 108.790, 0));
    values.push_back(proj_coord(530697.5914170, 4045466.9892900, 109.431, 0));
    values.push_back(proj_coord(530679.3685010, 4045367.7703640, 110.128, 0));
    printf("part2\n");
    values.push_back(proj_coord(531538.4354870, 4048509.9998010, 64.288, 0));
    values.push_back(proj_coord(531556.8995360, 4048547.5816830, 64.261, 0));
    values.push_back(proj_coord(531577.8245450, 4048592.6835390, 64.143, 0));
    values.push_back(proj_coord(531620.9541260, 4048682.7839080, 63.639, 0));
    printf("part3\n");
    values.push_back(proj_coord(532085.4049280, 4049653.0991560, 57.472, 0));
    values.push_back(proj_coord(532128.6695210, 4049743.1823270, 57.371, 0));
    printf("part4\n");
    values.push_back(proj_coord(534163.8825970, 4055941.5665400, 43.200, 0));
    values.push_back(proj_coord(534146.8050350, 4056040.1052300, 42.412, 0));

    values.push_back(proj_coord(534163.8825970, 4055941.5665400, 43.200, 0));
    values.push_back(proj_coord(534146.8050350, 4056040.1052300, 42.412, 0));

    values.push_back(proj_coord(533883.7245030, 4058322.4522400, 38.333, 0));
    values.push_back(proj_coord(533890.4302630, 4058416.3936440, 36.966, 0));
    values.push_back(proj_coord(533882.5973010, 4058516.3484610, 36.474, 0));
    values.push_back(proj_coord(533875.2326190, 4058611.4515880, 36.423, 0));

    values.push_back(proj_coord(533835.1025500, 4059124.4719580, 35.818, 0));
    values.push_back(proj_coord(533823.7337060, 4059223.0283020, 36.009, 0));
    values.push_back(proj_coord(533810.2798290, 4059322.7298090, 36.335, 0));
    values.push_back(proj_coord(533795.1248250, 4059422.0614390, 36.651, 0));
    values.push_back(proj_coord(533778.0002770, 4059520.7177840, 36.880, 0));
    values.push_back(proj_coord(533771.3996410, 4059560.7214340, 36.938, 0));
    double lam[3] = {0, 0, 0};
    double phi[3] = {0, 0, 0};
    for (int i = 0; i < values.size(); ++i)
    {
        /* transform to UTM zone 32, then back to geographical */
        b      = proj_trans(P, PJ_FWD, values[i]);
        lam[0] = b.lp.lam;
        lam[1] = (lam[0] - (int) lam[0]) * 60;
        lam[2] = (lam[1] - (int) lam[1]) * 60;
        phi[0] = b.lp.phi;
        phi[1] = (phi[0] - (int) phi[0]) * 60;
        phi[2] = (phi[1] - (int) phi[1]) * 60;

        printf("longitude: %d %d %.6f, latitude: %d %d %.6f\n",
               (int) lam[0],
               (int) lam[1],
               lam[2],
               (int) phi[0],
               (int) phi[1],
               phi[2]);
    }

    /* Clean up */
    proj_destroy(P);
    proj_context_destroy(C); /* may be omitted in the single threaded case */
    return 0;
}
