#include "camera.h"
#include "color.h"
#include "image.h"
#include "intersectable.h"
#include "light_graph.h"
#include "lighting.h"
#include "linear_types.h"
#include "material.h"
#include "plane.h"
#include "sphere.h"
#include "vector_ops.h"
#include <iostream>

using namespace linalg;

typedef std::tuple<
    std::size_t,
    std::size_t,
    vec3,
    vec3,
    vec2,
    Intersectable const* const>
    pixel_job_t;

int main()
{
    constexpr Screen screen{.discretization = {1000, 1000}, .size = {1.0, 1.0}};

    Camera camera(
        screen,
        1.0,
        {0, 4, -6},     // pos
        {0, -0.4, 1},   // view
        {0.0, 1.0, 0.0} // up
    );

    Material r_sphere_mat{
        .index_of_refraction = 1.0,
        .reflect_precent = 1.0,
        .refract_precent = 0.0,
        .ambient_color = {0, 0, 0},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.79,
        .diffuse_coeff = 0.2,
        .specular_coeff = 0.01,
        .specular_exponent = 100.0
    };
    Material ground_mat{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color = {0.5, 0.5, 0.5},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.2,
        .diffuse_coeff = 0.3,
        .specular_coeff = 0.5,
        .specular_exponent = 1.0
    };
    Material bg_material{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color = {0.5, 0.5, 1.0},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.2,
        .diffuse_coeff = 0.3,
        .specular_coeff = 0.5,
        .specular_exponent = 1.0
    };

    std::vector<std::tuple<Intersectable*, Material*>> os{};
    std::vector<std::tuple<Sphere, Material*>> spheres;
    spheres.push_back({Sphere({-2, 2, 0}, 2), &r_sphere_mat});
    spheres.push_back({Sphere({1, 2, 5}, 2), &r_sphere_mat});

    Material sm0{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9450244024808446, 0.26751175952367123, 0.5763168551199913},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 11.213443524847811
    };
    Material sm1{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6484432747360632, 0.9064619970469469, 0.23050499780950529},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 67.57062175876044
    };
    Material sm2{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8045002758136383, 0.866574663779417, 0.43137400842130247},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 7.819083976888031
    };
    Material sm3{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.47218821407381195, 0.780738324056633, 0.8397854145567455},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 20.42397939279299
    };
    Material sm4{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.0027381997142897907, 0.8542526258872226, 0.29053550169485365},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 60.890055860741064
    };
    Material sm5{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.13629315978150303, 0.7612490142143759, 0.06280906449741175},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 80.299240526228
    };
    Material sm6{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.08656998307505226, 0.08189205496546348, 0.3442229643282617},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 66.44763935859277
    };
    Material sm7{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3918717591820141, 0.31633449284742665, 0.36989360216392575},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 59.72130834966125
    };
    Material sm8{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.34202504633396835, 0.44790378239014295, 0.8463576092404358},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 66.80948639878497
    };
    Material sm9{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8801796914112424, 0.766163751046724, 0.4891481539795105},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 37.81317775728093
    };
    Material sm10{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7299218113948108, 0.9313390808306339, 0.28050293114495517},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 96.03539794750743
    };
    Material sm11{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.27418684987235487, 0.9845424613497477, 0.7775408139502212},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 57.45972338647576
    };
    Material sm12{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.734774360931393, 0.013605959880671481, 0.462804306482702},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 90.23424138781303
    };
    Material sm13{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.39348876155478896, 0.48099349988197593, 0.117391963575882},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 33.21856101081187
    };
    Material sm14{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6010674240397624, 0.2891012835050376, 0.0942069189819339},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 48.20781065561381
    };
    Material sm15{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3348012909103715, 0.38942337079908207, 0.9484033125459029},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 51.63206212102056
    };
    Material sm16{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6960925867032554, 0.6408702859152318, 0.7670828646246608},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 75.8247672805747
    };
    Material sm17{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.45682601696147673, 0.6496441274890595, 0.1841128077048848},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 6.7324417028004735
    };
    Material sm18{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4756295159632782, 0.00376811437176916, 0.7899937779678664},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 3.5120957121078655
    };
    Material sm19{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.18602481977903573, 0.39709488842531815, 0.8312917413318937},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 37.624827464851606
    };
    Material sm20{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8369822056031931, 0.5275367240206061, 0.6159663954627427},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 75.45200100296219
    };
    Material sm21{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3911784841263304, 0.6965840114265118, 0.33288688351932894},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 22.632819055540388
    };
    Material sm22{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4658464559246581, 0.7801402406471971, 0.20310422761557456},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 49.21819617294663
    };
    Material sm23{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.07234363074476291, 0.617020413660972, 0.495049145352237},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 76.41435088092065
    };
    Material sm24{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.12758870167659453, 0.28395720778780764, 0.1284581390553582},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 50.6022719948794
    };
    Material sm25{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4668463012632994, 0.38414573033478594, 0.29381035453434645},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 34.63850503749335
    };
    Material sm26{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4784447163757122, 0.23932429310949555, 0.7325044585236222},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 96.24434977962669
    };
    Material sm27{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8584645165556616, 0.330005055636878, 0.21948460823173166},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 51.54494512809226
    };
    Material sm28{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3542660890080058, 0.8011983100904219, 0.06133692099921639},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 66.56388088304962
    };
    Material sm29{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.25165896835875046, 0.2619957446430935, 0.8769171918858126},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 15.063750520059704
    };
    Material sm30{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3300165583107417, 0.6501505238447672, 0.16215065885188207},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 84.12445748993686
    };
    Material sm31{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5458750737002083, 0.3868629666653475, 0.14709239937585106},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 63.37015341632427
    };
    Material sm32{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9738230856706627, 0.7203763188587177, 0.6003321654338022},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 28.523067102819923
    };
    Material sm33{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.35408379576367277, 0.9567592830771138, 0.653399734270235},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 63.50126213712555
    };
    Material sm34{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.39481573179126195, 0.4125075606769376, 0.7913697498775212},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 85.18865471998568
    };
    Material sm35{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.47698072197583763, 0.04426400723459434, 0.8411932587387037},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 42.89350836514969
    };
    Material sm36{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5188999139398115, 0.10916858031003707, 0.006506360534928146},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 67.32969923992994
    };
    Material sm37{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6752718222192592, 0.47164849859610025, 0.08194910111257714},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 26.88500570904394
    };
    Material sm38{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.15239507342585146, 0.07577975948639681, 0.5151494699957297},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 51.134014312158286
    };
    Material sm39{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.28866434327205315, 0.8732432953629913, 0.3730547867134103},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 53.44415213007028
    };
    Material sm40{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7270079059962508, 0.28056802166906136, 0.9140241696321444},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 8.894028498366314
    };
    Material sm41{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.38240983964483477, 0.16743536193000863, 0.5130478215170318},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 43.211852235008294
    };
    Material sm42{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7598764189586751, 0.7158025228794908, 0.25532197776751997},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 57.57098235609715
    };
    Material sm43{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.021606983493574705, 0.6226595852068161, 0.23398893947820754},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 66.32763800014712
    };
    Material sm44{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.93476943488939, 0.6398624057446926, 0.4423866601056551},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 88.27938236831227
    };
    Material sm45{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9290202012316665, 0.37340378105447114, 0.32222151914876795},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 2.3599938748917526
    };
    Material sm46{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8742882106885196, 0.12182352814637709, 0.38240757098567835},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 62.71171365149472
    };
    Material sm47{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4909542310326914, 0.28883148603534936, 0.41951681453775225},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 60.28048261006771
    };
    Material sm48{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.00015761129561642218, 0.4926752961428048, 0.7444210671250107},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 13.809052196881966
    };
    Material sm49{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4198589399415462, 0.9683150776797728, 0.7932868297490644},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 58.891186288162146
    };
    Material sm50{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.20237201251611348, 0.07512616098577485, 0.2848983608536192},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 39.754134519104745
    };
    Material sm51{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.32001325292451777, 0.7712145900703095, 0.8567048770781014},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 43.9682965631756
    };
    Material sm52{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5615198047116801, 0.02974768132118555, 0.8592703645862794},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 37.24009215015265
    };
    Material sm53{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9614979514996168, 0.6197711863576921, 0.8671647143518825},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 93.55794323964352
    };
    Material sm54{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5251083713635017, 0.3017246011524871, 0.769615826991736},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 12.060679344444903
    };
    Material sm55{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.002449943241513375, 0.7101462451339114, 0.42827250572291164},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 25.396123600475494
    };
    Material sm56{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.11550825628302863, 0.826755876275634, 0.3057032462267395},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 58.63248695467854
    };
    Material sm57{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.35699043822414855, 0.7967365505269687, 0.5257932057267853},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 33.7228802213021
    };
    Material sm58{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8824144506940325, 0.3193644693834473, 0.9545803107537811},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 22.715012294702507
    };
    Material sm59{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.2765351154828214, 0.4021545939689668, 0.47671917415052256},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 47.219594857645816
    };
    Material sm60{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.04013184323706975, 0.9891188450138169, 0.5766434061762646},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 93.53676221440226
    };
    Material sm61{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5476073285287079, 0.05132892982926518, 0.12184130047621988},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 69.74126662385923
    };
    Material sm62{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6892607958408541, 0.26295978804473197, 0.5651395802765764},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 90.02531689203978
    };
    Material sm63{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9807653606280943, 0.12344839940336605, 0.6474046541912477},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 8.861539758251258
    };
    Material sm64{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.11227548091098294, 0.8664846308887645, 0.9009448362567578},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 20.93030059371026
    };
    Material sm65{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.303603594417317, 0.45423608619772216, 0.08812142457906635},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 65.19731474373953
    };
    Material sm66{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.682020869955962, 0.41225193313522934, 0.3006477272051563},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 53.37352505318799
    };
    Material sm67{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7080110617942131, 0.013747050673407757, 0.9432791039700763},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 92.55373386465115
    };
    Material sm68{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4750540711244986, 0.16237355037918844, 0.2396643250995255},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 52.25583044751135
    };
    Material sm69{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.09157476707138723, 0.6033279276194442, 0.9987219724752354},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 98.16940338194998
    };
    Material sm70{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.70392283520149, 0.9869009751726338, 0.9629896645941688},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 24.099627661111434
    };
    Material sm71{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.36085587674973874, 0.32380528877799464, 0.5109001749826093},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 0.5258909431963721
    };
    Material sm72{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.12452758399574027, 0.15402499417268745, 0.3882063826672748},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 79.65620889221897
    };
    Material sm73{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4738138473768888, 0.6584241015549733, 0.9755650310066579},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 48.661087651272915
    };
    Material sm74{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6797100391333369, 0.9236874538396748, 0.24696554511865254},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 60.78842419899087
    };
    Material sm75{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9692174825568202, 0.7540079623199408, 0.3388359572393933},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 83.14155920899466
    };
    Material sm76{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8268218292365165, 0.4504074082117322, 0.7325782464009483},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 64.07321639046589
    };
    Material sm77{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8532735533264505, 0.3803743847637878, 0.019848477794866204},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 28.259434514831106
    };
    Material sm78{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3128542200765114, 0.3392904333746297, 0.38689704157000293},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 11.795711429720502
    };
    Material sm79{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.07347972015129745, 0.6851248875185901, 0.9073851708909604},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 32.79335786991746
    };
    Material sm80{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6864430771240236, 0.7627161537583206, 0.5667490580499193},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 80.12209391440524
    };
    Material sm81{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9552327447516473, 0.39948773697534223, 0.7145905928007222},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 88.09044124701117
    };
    Material sm82{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6427265436312778, 0.21766606323943538, 0.034182648241920965},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 42.40105865365088
    };
    Material sm83{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6842354808008128, 0.34638171447281896, 0.08669541428144811},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 92.30577562134614
    };
    Material sm84{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.44143930225807027, 0.33244127194383466, 0.9766662873932154},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 5.9178233976999834
    };
    Material sm85{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7167350182921588, 0.7761196199345984, 0.7295574441680679},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 95.64012267514607
    };
    Material sm86{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.873325764911812, 0.2913590955215125, 0.2847287689804172},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 8.11131657693287
    };
    Material sm87{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.11496631951937686, 0.9869401120082767, 0.42395814772819285},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 62.60592238798127
    };
    Material sm88{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5810161769781783, 0.9104818976998149, 0.29494917499374373},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 28.239434583381428
    };
    Material sm89{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5253822591747511, 0.6478244606891902, 0.2930116002687675},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 66.27065696295917
    };
    Material sm90{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.2467537487529613, 0.9368337728624692, 0.35778485282986183},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 10.13391934014798
    };
    Material sm91{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6304446298643557, 0.9585326860371594, 0.9256216479825233},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 73.14166984499147
    };
    Material sm92{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.358352564699877, 0.37513271124563397, 0.6252956231086949},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 30.663924233186314
    };
    Material sm93{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.909540161197209, 0.8961252411246432, 0.9390528496465551},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 72.32267398086623
    };
    Material sm94{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.29935989885969927, 0.6271341859463256, 0.3728640828534291},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 50.269835741687
    };
    Material sm95{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7200003934501343, 0.8171624666611761, 0.3536841311440623},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 99.00970636652525
    };
    Material sm96{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.03905268692850605, 0.2082790319530523, 0.33393849022538613},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 61.04478456714679
    };
    Material sm97{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.17420953542492068, 0.8659056767486896, 0.6084144434023218},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 37.736130474216466
    };
    Material sm98{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8985855714259844, 0.20340759186540058, 0.20836836319860852},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 14.295142000575856
    };
    Material sm99{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6040042896177448, 0.41530392508717506, 0.04639741768476424},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 2.835825114800339
    };
    Material sm100{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9652124618427524, 0.5073877559433264, 0.3924257162798198},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 15.700202502082783
    };
    Material sm101{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6677983185127679, 0.39371617324851926, 0.9317769606470668},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 19.096312127251625
    };
    Material sm102{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5057854850753624, 0.1298481513207872, 0.9387754339147096},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 76.66631496195645
    };
    Material sm103{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.08224484520106257, 0.18846940779035526, 0.6567799263233384},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 41.48975409811519
    };
    Material sm104{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7938035295796199, 0.050973998819210165, 0.8772887597942739},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 25.286311228080105
    };
    Material sm105{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.21514033860680337, 0.5516547009563108, 0.7179197061645719},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 60.109965232804505
    };
    Material sm106{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.04919878226092289, 0.03948044693182773, 0.11882937989273856},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 19.81054337735546
    };
    Material sm107{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8172977301543095, 0.5977514603093651, 0.6487940232150741},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 79.61588591371608
    };
    Material sm108{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7600153332710211, 0.4342366210091555, 0.4973847597452188},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 69.71737508765112
    };
    Material sm109{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.31417746994303275, 0.9433311671752678, 0.16205363118403882},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 27.538114389786706
    };
    Material sm110{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.09559832130881019, 0.09717443176669915, 0.6549700008377484},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 76.9293447683674
    };
    Material sm111{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6723120120633385, 0.93679505133071, 0.6594937267547111},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 98.48775872131428
    };
    Material sm112{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.2904904959442185, 0.4176756581475506, 0.12468862511040224},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 40.18861792893064
    };
    Material sm113{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9420078361190715, 0.19809230423838664, 0.8587827464947391},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 94.73772515683294
    };
    Material sm114{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7083184778223846, 0.49612083028449827, 0.6902391206525916},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 72.66279570683854
    };
    Material sm115{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5184060699714591, 0.35658578115894835, 0.04215874277182241},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 72.62499905979269
    };
    Material sm116{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3902889255637202, 0.5910259316480537, 0.7644401775128284},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 7.176504776863024
    };
    Material sm117{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.34937581558957775, 0.4451963156929323, 0.6915070930214554},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 82.91054716352882
    };
    Material sm118{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.34919214416560085, 0.6660315684629292, 0.051667320415307305},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 85.3725826473236
    };
    Material sm119{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4920839169986232, 0.020410789953495878, 0.29764242209479475},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 17.03356925764641
    };
    Material sm120{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.26579128354370085, 0.7294215183010414, 0.15279122521138888},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 88.74517003647996
    };
    Material sm121{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8648285748876089, 0.98472214912285, 0.024310416499947474},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 10.284535253160632
    };
    Material sm122{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.591655853482163, 0.5876126723813069, 0.6476192323357656},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 31.16432420555083
    };
    Material sm123{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.14680997442056543, 0.6091995747424819, 0.6833233142737225},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 94.92321833988606
    };
    Material sm124{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6841642570830984, 0.6755143288486315, 0.3610563339491297},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 18.430499681327873
    };
    Material sm125{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.1961491868069163, 0.4618638490458933, 0.11248806766858455},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 14.644141885539652
    };
    Material sm126{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6172776262813193, 0.6739192680507182, 0.3233180093167517},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 61.394845928411236
    };
    Material sm127{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.23646025379388602, 0.959745901139127, 0.05181703814902583},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 42.26193730685156
    };
    Material sm128{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.1385721585394758, 0.23399539413023585, 0.5059813446810753},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 37.539086762304265
    };
    Material sm129{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6254676313153283, 0.8023683084401997, 0.8172681640605625},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 78.39055167512642
    };
    Material sm130{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.1352699538607367, 0.1200186140434385, 0.04738705245777908},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 36.673459320872105
    };
    Material sm131{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3641554024471507, 0.9904524870037853, 0.23528105954188616},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 76.68701999618771
    };
    Material sm132{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.2537511507972824, 0.900761713540863, 0.17998506600371222},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 19.627871366445515
    };
    Material sm133{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8586397656977344, 0.7604976467125953, 0.0028836226747488425},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 30.595798731733204
    };
    Material sm134{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5178412072318547, 0.7440253861182154, 0.7572600003482374},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 44.792824217289095
    };
    Material sm135{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.2982030561504495, 0.5382277104297534, 0.3796670804657589},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 72.58531886958751
    };
    Material sm136{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5007512028728313, 0.5891517447127654, 0.0123336393813398},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 76.47087772000896
    };
    Material sm137{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.30186207279106114, 0.8635747824273593, 0.776379973633726},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 71.68305926695281
    };
    Material sm138{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.2784595856756641, 0.25656654676294544, 0.07479102131114523},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 0.9597553641192746
    };
    Material sm139{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8471343562850019, 0.9542937335915078, 0.33619059234957904},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 55.01941395399794
    };
    Material sm140{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.1390029444202182, 0.30805213220512195, 0.06119928255035878},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 57.316230694491054
    };
    Material sm141{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.28349804480519636, 0.1227222129612805, 0.4279249113134659},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 81.4709073264932
    };
    Material sm142{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6064622547927766, 0.5814735225552414, 0.9547576002020874},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 25.87871403697767
    };
    Material sm143{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6365341303159792, 0.19688268220071758, 0.18216109939254888},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 38.14531532655927
    };
    Material sm144{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4233560289953425, 0.58877278864334, 0.07952309023213822},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 48.42696938353331
    };
    Material sm145{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6165497251520051, 0.23523692550217667, 0.39745214589248423},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 9.89985516772377
    };
    Material sm146{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.233758781942787, 0.9853359520104099, 0.28235139352249605},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 0.1254005300473371
    };
    Material sm147{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.22851491677417846, 0.05593293489101492, 0.924156265568667},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 82.92415048843694
    };
    Material sm148{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7794443135284517, 0.3203389147560328, 0.2940104138756291},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 80.23593417095407
    };
    Material sm149{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.19769608578714826, 0.3975972297146573, 0.07720242506579578},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 21.98005644872838
    };
    Material sm150{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9092428998627026, 0.24500663765247355, 0.14137216046655454},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 78.21221778520847
    };
    Material sm151{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7040139260162245, 0.734411678763691, 0.12664526901576234},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 77.41108975921259
    };
    Material sm152{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.34954826094807634, 0.6075827774840777, 0.5421179385674487},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 62.74851483859466
    };
    Material sm153{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.44882022186631076, 0.8322982598457985, 0.40505513947593696},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 24.850252876123136
    };
    Material sm154{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.1476849596155071, 0.03517173184499689, 0.21830671330002793},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 88.75161551888984
    };
    Material sm155{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4604761870975892, 0.30268006768613276, 0.3515792057837914},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 30.0719201926264
    };
    Material sm156{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8869476483582012, 0.9864106877137029, 0.7895184783254736},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 43.05742247811941
    };
    Material sm157{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8889583850983765, 0.26334872252947783, 0.3895836324661932},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 4.144077763476539
    };
    Material sm158{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.1570242062397441, 0.8862855631810695, 0.45022806460115705},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 75.48942143024509
    };
    Material sm159{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6836440179790022, 0.3037597395102033, 0.2556176603551116},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 15.663358986271147
    };
    Material sm160{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5671641738495531, 0.5943301586997248, 0.3045897555113539},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 43.26974787153337
    };
    Material sm161{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.10071163517607185, 0.4656644250611516, 0.9408719364404501},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 22.76445614222903
    };
    Material sm162{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9939057619832347, 0.7647044258920833, 0.12513841824147176},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 70.45021737281184
    };
    Material sm163{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6445672593489706, 0.692862945889925, 0.3166832269967259},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 90.47750771388442
    };
    Material sm164{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4935016872074195, 0.30725900036337317, 0.9413112708824943},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 90.48663039616422
    };
    Material sm165{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7792600635402537, 0.883694596872295, 0.2662178805936266},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 19.50983645290907
    };
    Material sm166{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.28653898625770036, 0.48903380846011324, 0.5000675111097239},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 90.85703746055644
    };
    Material sm167{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.516783972569717, 0.5186418493030479, 0.4722015706877878},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 30.481076621090264
    };
    Material sm168{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5235625850890618, 0.10745882792071926, 0.20998739260077004},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 86.47730810119951
    };
    Material sm169{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7506088448997192, 0.7557216924497837, 0.7724783593943192},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 40.2915429596315
    };
    Material sm170{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.05567991548017115, 0.07243169573403996, 0.7572345403931271},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 66.8615785800438
    };
    Material sm171{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5993948669409157, 0.9892234400020962, 0.0926525784004828},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 74.65264464391839
    };
    Material sm172{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5588432148286309, 0.1149574351637781, 0.8531295708224037},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 29.786388984364542
    };
    Material sm173{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9083159730684752, 0.6835449805118858, 0.4427471978425018},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 0.07324741990921702
    };
    Material sm174{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7670080859025423, 0.5754061606672174, 0.0021290329403726993},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 68.23738813542731
    };
    Material sm175{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4828601926857021, 0.06081681793381155, 0.4202776500952975},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 7.0345936875334525
    };
    Material sm176{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6826802655760418, 0.9533136501649612, 0.2051747188567381},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 43.884646581778576
    };
    Material sm177{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.915103924404174, 0.06110133523906258, 0.930541681183346},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 16.300119156271176
    };
    Material sm178{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4649390567667101, 0.09612277937243086, 0.7045593514504199},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 2.391401866752829
    };
    Material sm179{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4664213592977038, 0.7729067881058536, 0.3508025715240727},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 86.2125457798052
    };
    Material sm180{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.041339450388450216, 0.4626176189331207, 0.033599145038194944},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 6.505299731398074
    };
    Material sm181{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6687481256498474, 0.3462713028123311, 0.4372049306523135},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 94.01622577842811
    };
    Material sm182{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3156273280206109, 0.3253420266542143, 0.6339411791696695},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 41.505439547455495
    };
    Material sm183{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.636008283383912, 0.6789452531344186, 0.28007895266687366},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 26.0073430891418
    };
    Material sm184{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.14643664653467336, 0.10112141227708416, 0.5261489657095402},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 23.946899599617556
    };
    Material sm185{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6822925394117785, 0.9676187787826698, 0.9989273175070735},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 31.588265556177763
    };
    Material sm186{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8095490597287534, 0.28172893487293205, 0.3467524873118033},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 98.28633590168091
    };
    Material sm187{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.16319391867963728, 0.48677669409714064, 0.4911813211714572},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 43.88215978390243
    };
    Material sm188{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6476019650716656, 0.6919447190237524, 0.7934683834258218},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 68.31496918649526
    };
    Material sm189{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8631141871162311, 0.9656740168475456, 0.390244578857163},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 78.89171341086657
    };
    Material sm190{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.33050204044711606, 0.7590884036455279, 0.01709785266810737},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 84.41952884430287
    };
    Material sm191{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4048995252415162, 0.31780530915096883, 0.24089942966567868},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 31.95599013990037
    };
    Material sm192{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8285842946518155, 0.02819163142259773, 0.8462732687580976},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 60.72138483464753
    };
    Material sm193{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3635516808532073, 0.24700419109374894, 0.3238534353295982},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 32.771025839603254
    };
    Material sm194{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4265215363355561, 0.8578786631125993, 0.9059866279579345},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 27.64287705914833
    };
    Material sm195{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6058744710628423, 0.560456604683534, 0.6579132344134399},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 20.554211570496673
    };
    Material sm196{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9706551779860221, 0.8940453337481746, 0.3867469790705357},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 30.272319706891317
    };
    Material sm197{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.2344598995209194, 0.4499949046278262, 0.3588750355107334},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 78.69980812353566
    };
    Material sm198{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3582458661558975, 0.1322264587476777, 0.3920873372949456},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 66.65813607340156
    };
    Material sm199{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.578016065651432, 0.8033457157518421, 0.01710284779349558},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 37.90360906722496
    };
    Material sm200{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.42300361668797515, 0.9680084346424865, 0.7486949061488838},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 41.84548883288557
    };
    Material sm201{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.33679574821193836, 0.47137340421365403, 0.6010555210591488},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 58.857340564379136
    };
    Material sm202{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9396884919559227, 0.22751568003141487, 0.3372664649650443},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 91.5068639700321
    };
    Material sm203{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5050290126563977, 0.9222650701561698, 0.5522516965863598},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 74.49074482887958
    };
    Material sm204{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.2360000020549773, 0.015043598446065176, 0.4918544577023185},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 69.23655841383768
    };
    Material sm205{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.756345463705277, 0.7193534878581963, 0.8730475351891706},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 82.68721238693064
    };
    Material sm206{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9488991065076547, 0.5495785984867502, 0.5998787833474235},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 30.827507130036537
    };
    Material sm207{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5695380669416595, 0.9868482066398847, 0.8546507186071156},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 60.118787529061
    };
    Material sm208{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5543329760898825, 0.1620191916696636, 0.7521362818621131},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 63.02739650606879
    };
    Material sm209{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.766063779831095, 0.12275624523036388, 0.3264198108592262},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 75.40421986370808
    };
    Material sm210{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.1721030274530181, 0.9775524602841124, 0.4887358136252209},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 29.61579784393703
    };
    Material sm211{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5006672432390651, 0.16168549192419257, 0.832815204610465},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 32.37608327774586
    };
    Material sm212{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4010610693423482, 0.2601914746350181, 0.271677971341462},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 72.34786669829595
    };
    Material sm213{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.1507032637194211, 0.7560415411332636, 0.598424234976574},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 55.59226935355722
    };
    Material sm214{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9284685904110279, 0.2498754726629273, 0.41853571875178286},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 33.79541092379716
    };
    Material sm215{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.30138102421572044, 0.28777531646608856, 0.6532795454046025},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 50.77035767540289
    };
    Material sm216{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.012700624582209663, 0.36199288309837574, 0.8215454432590623},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 28.673916379779442
    };
    Material sm217{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8382867438473297, 0.25886015599794543, 0.4044438541185109},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 66.44707569785263
    };
    Material sm218{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.08285905861020659, 0.7262705430107808, 0.8486056848473422},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 31.02797583886239
    };
    Material sm219{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.19194112650866302, 0.8401937349731328, 0.6079765548020164},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 77.80698820101706
    };
    Material sm220{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.619652163980924, 0.608348919159882, 0.37290314854306716},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 8.074692495389545
    };
    Material sm221{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.0719943154215662, 0.36386856232056486, 0.8802662705927979},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 51.47355394697816
    };
    Material sm222{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5854775567618615, 0.14650829225515316, 0.1871423443403898},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 55.76575850493979
    };
    Material sm223{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7493262044714869, 0.5334054513715946, 0.7310008278044281},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 64.15659931936175
    };
    Material sm224{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3889512715523403, 0.8671213355655484, 0.7669053772670954},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 7.620282559023462
    };
    Material sm225{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.10941700042142222, 0.7414178106757265, 0.6731585790754884},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 68.16977780961085
    };
    Material sm226{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3474157871498982, 0.5333251851026442, 0.07158006039676879},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 67.19192051107994
    };
    Material sm227{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8010787596158242, 0.9712951625404322, 0.7181337167291888},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 47.87680279238078
    };
    Material sm228{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.17884067107992085, 0.916864895559078, 0.5079088453468255},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 13.813537109961139
    };
    Material sm229{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3015221195266885, 0.9635562180176981, 0.7774624667789827},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 27.10165967067486
    };
    Material sm230{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9912440526448454, 0.24502875494877052, 0.966612847127823},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 25.885672471091237
    };
    Material sm231{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.2978105298659057, 0.9449743270419424, 0.32389988974035056},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 27.551695067794178
    };
    Material sm232{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6125204638833237, 0.5489033705721731, 0.5509425119426989},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 17.78727110409557
    };
    Material sm233{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.19167687679516798, 0.07055217429792049, 0.42441750179936844},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 5.5782090632034205
    };
    Material sm234{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3722456936809073, 0.6187826406734477, 0.5502410879093422},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 36.117589501192725
    };
    Material sm235{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5266361905559587, 0.9468845256691278, 0.9819728927552835},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 26.794238664895964
    };
    Material sm236{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3023505522667971, 0.5794122022314951, 0.6254994981635273},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 57.16835362966807
    };
    Material sm237{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.294966192305365, 0.2172454697225784, 0.8115975016070464},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 21.81827543786725
    };
    Material sm238{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.724872010384554, 0.5333904919336143, 0.5577930593017855},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 76.35554355833867
    };
    Material sm239{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.35133012982198164, 0.8144581806642158, 0.5390066405936864},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 65.71800668155218
    };
    Material sm240{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6936145280649751, 0.06051840991009294, 0.900773572485473},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 31.612598055176242
    };
    Material sm241{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.027158813479542077, 0.5447981492890599, 0.6161581541758991},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 39.733520505909134
    };
    Material sm242{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.1902927182784161, 0.8728307277803166, 0.6115633677699736},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 39.96314548065478
    };
    Material sm243{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.1532202924356254, 0.09419229274435503, 0.07420385804709906},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 67.34147877575666
    };
    Material sm244{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8284734486668702, 0.013235481519275227, 0.673961989128903},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 43.384955993865404
    };
    Material sm245{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.30471486271560555, 0.45108330976067457, 0.48134400928903676},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 51.11667831225635
    };
    Material sm246{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8490900852509349, 0.47202252958338253, 0.5927951459703138},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 84.74706021724131
    };
    Material sm247{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6042712319651689, 0.9825249082236787, 0.8731410850203345},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 12.612171834917218
    };
    Material sm248{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9547524711070116, 0.2106608319169293, 0.5880426851953926},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 0.9354656560190855
    };
    Material sm249{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5670571836479704, 0.8480548890033109, 0.754701144742235},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 61.1186009495306
    };
    Material sm250{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9317112310718898, 0.4009440941170864, 0.1490478327235365},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 75.8286084227767
    };
    Material sm251{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5540898321873016, 0.09416626521298044, 0.13707533784902237},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 57.181553998056124
    };
    Material sm252{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.45103538353816863, 0.5483586935147406, 0.879300256952766},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 70.74311864850986
    };
    Material sm253{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9375118987710964, 0.724886279328912, 0.33977403715178434},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 41.10983951851822
    };
    Material sm254{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5482040360619003, 0.2873124881976369, 0.30783951898405604},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 35.191144805148966
    };
    Material sm255{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.35773923459575285, 0.032877806347538785, 0.5911600159886296},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 7.339034886818229
    };
    Material sm256{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.547420603008334, 0.1589317804799939, 0.35811288467547475},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 17.916507804817872
    };
    Material sm257{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.0973039937877267, 0.30419461342927834, 0.8042874020362257},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 48.739949620578194
    };
    Material sm258{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.2638862871247426, 0.8797402186531807, 0.6649435206993842},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 53.359745232488756
    };
    Material sm259{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6384604791248754, 0.12187394632772053, 0.9640277695846933},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 98.53463970075438
    };
    Material sm260{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.15702094846757153, 0.38980618749746865, 0.9740600057204765},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 67.2208117142301
    };
    Material sm261{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.2824653052717012, 0.34696697890343986, 0.32755836904806046},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 90.6915422869843
    };
    Material sm262{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.28577684312774077, 0.4917696774561191, 0.4882935747809456},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 16.05410165946074
    };
    Material sm263{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8940696560517636, 0.9971042045025716, 0.25075696368908096},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 79.08082847173738
    };
    Material sm264{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3489036457322183, 0.018591063458276436, 0.9345056471404259},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 6.927667635014334
    };
    Material sm265{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.715610020611914, 0.2881996778553312, 0.7089112732116897},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 54.34620503071087
    };
    Material sm266{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4776211679874851, 0.8852672517414868, 0.254593941093734},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 77.44292801574095
    };
    Material sm267{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.004474082043923966, 0.3173779727080608, 0.9331443728492804},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 88.57850221047599
    };
    Material sm268{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8747773264075385, 0.8954830099711037, 0.00035054090509212266},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 53.7335018229924
    };
    Material sm269{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5158472907640592, 0.4847319104368196, 0.2045330639443993},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 16.87693228239352
    };
    Material sm270{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.34325604714556035, 0.5027607803607311, 0.7186801359877563},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 40.53611497204758
    };
    Material sm271{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.5031158131085643, 0.9613286337385929, 0.5301394661379585},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 33.80607538769602
    };
    Material sm272{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.8522888942331289, 0.08297561944203702, 0.14353847037643241},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 8.750865842447697
    };
    Material sm273{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9129131753791272, 0.32230522645746273, 0.488452381604759},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 48.545372102106576
    };
    Material sm274{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6070099805585731, 0.180483803674799, 0.09855338549479109},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 62.702789798575175
    };
    Material sm275{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.146913296575359, 0.9518844519946714, 0.6953905830715027},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 38.52644590215276
    };
    Material sm276{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.15545748971385942, 0.36075583942426714, 0.88415704177307},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 3.4800307887933757
    };
    Material sm277{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.48509667753644303, 0.2942969783810464, 0.7627360704029109},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 68.10031850446016
    };
    Material sm278{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.3200802227950623, 0.7381731614846969, 0.35141632060416694},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 94.96587657635061
    };
    Material sm279{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.08695194841781817, 0.3210277485367986, 0.6230061538912007},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 93.46293242168753
    };
    Material sm280{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.888811822739957, 0.20485574974704257, 0.08127525938094382},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 39.078349756631745
    };
    Material sm281{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.2813097955343472, 0.6537275916575981, 0.5815042829817928},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 29.892261133553998
    };
    Material sm282{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.09296529021197841, 0.24006668514519658, 0.31685628093121276},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 58.52495869602552
    };
    Material sm283{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.1520025165317389, 0.7523501451768151, 0.28081887162675645},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 78.81578097835295
    };
    Material sm284{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7135355225740787, 0.5475222912981913, 0.6577343735175005},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 17.026493815655474
    };
    Material sm285{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.4974110271346289, 0.47053749067020045, 0.2457433038884943},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 95.0683007619673
    };
    Material sm286{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7898355307653677, 0.5729559197543328, 0.4400448604930366},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 75.04641765814532
    };
    Material sm287{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.04361204390259876, 0.10337506000829078, 0.9565459841969933},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 6.651305914496863
    };
    Material sm288{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.26694044317058085, 0.4245510279294247, 0.019963972455562917},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 88.5591199234693
    };
    Material sm289{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6430291450437773, 0.01737229111820726, 0.2222404834970273},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 70.49345805722204
    };
    Material sm290{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7255976914175709, 0.3914610143771696, 0.5102707612676322},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 63.85348970066042
    };
    Material sm291{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.673417841364662, 0.9603381944183919, 0.3678875161533147},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 44.46501875103242
    };
    Material sm292{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.7729670494294336, 0.8000451362643402, 0.5445147823119695},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 36.1972332889515
    };
    Material sm293{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.26163306346679793, 0.4459098578313182, 0.19253494297039997},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 90.41268244663588
    };
    Material sm294{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.9670656796862424, 0.48555764089368203, 0.7674643411322416},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 1.7284243930790755
    };
    Material sm295{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6372176207312794, 0.8337698318956773, 0.28120934706266865},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 62.01973476030527
    };
    Material sm296{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.1318448169095997, 0.056531815713100175, 0.4970090938826497},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 46.119832700491834
    };
    Material sm297{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6545357925255783, 0.9301045168564446, 0.7195231870066432},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 51.83653553624259
    };
    Material sm298{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.6795179063837483, 0.6957914958578998, 0.9283183899380438},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 46.34612261917926
    };
    Material sm299{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color =
            {0.29982172563365383, 0.39177364837542716, 0.7261997522922496},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.4,
        .specular_coeff = 0.1,
        .specular_exponent = 88.87149468412058
    };
    spheres.push_back(
        {Sphere(
             {-14.212081244705004, 0.15984320839213806, -1.2583674031540752},
             0.15984320839213806
         ),
         &sm0}
    );
    spheres.push_back(
        {Sphere(
             {-6.442911748999672, 0.5398077836067031, 3.830562455507467},
             0.5398077836067031
         ),
         &sm1}
    );
    spheres.push_back(
        {Sphere(
             {11.203337314827, 0.469720208370187, 5.487794688848405},
             0.469720208370187
         ),
         &sm2}
    );
    spheres.push_back(
        {Sphere(
             {7.551985643828004, 0.47768419903222764, 7.62452514326379},
             0.47768419903222764
         ),
         &sm3}
    );
    spheres.push_back(
        {Sphere(
             {13.102738578943839, 0.14342051176626353, -4.740161885171895},
             0.14342051176626353
         ),
         &sm4}
    );
    spheres.push_back(
        {Sphere(
             {-3.388457434926174, 0.32042308154809884, -2.3430492900515585},
             0.32042308154809884
         ),
         &sm5}
    );
    spheres.push_back(
        {Sphere(
             {-7.41365531743818, 0.5142520967584681, 10.435556672878867},
             0.5142520967584681
         ),
         &sm6}
    );
    spheres.push_back(
        {Sphere(
             {-9.000479230829102, 0.4561718357388105, -4.449091737843711},
             0.4561718357388105
         ),
         &sm7}
    );
    spheres.push_back(
        {Sphere(
             {-7.503384764399611, 0.06337557566591741, -7.49940268492784},
             0.06337557566591741
         ),
         &sm8}
    );
    spheres.push_back(
        {Sphere(
             {-4.885101670429631, 0.2547355217142143, 18.009971070261905},
             0.2547355217142143
         ),
         &sm9}
    );
    spheres.push_back(
        {Sphere(
             {6.509137973688372, 0.21348602860980515, 10.525293108654235},
             0.21348602860980515
         ),
         &sm10}
    );
    spheres.push_back(
        {Sphere(
             {6.568326860847822, 0.2806355049881637, 6.026990760569614},
             0.2806355049881637
         ),
         &sm11}
    );
    spheres.push_back(
        {Sphere(
             {14.862097896357636, 0.21938218355513378, -6.448433909882977},
             0.21938218355513378
         ),
         &sm12}
    );
    spheres.push_back(
        {Sphere(
             {-7.087734762183702, 0.49890529538947226, 10.395679767804317},
             0.49890529538947226
         ),
         &sm13}
    );
    spheres.push_back(
        {Sphere(
             {-12.809316239816035, 0.06024575396196642, -6.513508311527488},
             0.06024575396196642
         ),
         &sm14}
    );
    spheres.push_back(
        {Sphere(
             {4.661920707061402, 0.5104660990230218, 9.546269312080359},
             0.5104660990230218
         ),
         &sm15}
    );
    spheres.push_back(
        {Sphere(
             {-7.401328403250771, 0.10827481703007592, -3.7064134691488606},
             0.10827481703007592
         ),
         &sm16}
    );
    spheres.push_back(
        {Sphere(
             {12.763710778789509, 0.27295645657638123, 14.439846557911729},
             0.27295645657638123
         ),
         &sm17}
    );
    spheres.push_back(
        {Sphere(
             {12.650049820709715, 0.19150208696208354, 20.509485214029716},
             0.19150208696208354
         ),
         &sm18}
    );
    spheres.push_back(
        {Sphere(
             {-0.0513372536752712, 0.29726446475408425, 11.40402023908258},
             0.29726446475408425
         ),
         &sm19}
    );
    spheres.push_back(
        {Sphere(
             {-4.150503137178417, 0.07643977672012776, -7.048342239862936},
             0.07643977672012776
         ),
         &sm20}
    );
    spheres.push_back(
        {Sphere(
             {2.334263779201187, 0.3431176566487705, 3.9722823720008407},
             0.3431176566487705
         ),
         &sm21}
    );
    spheres.push_back(
        {Sphere(
             {0.6013513598375049, 0.13038423517680592, 12.053846965679519},
             0.13038423517680592
         ),
         &sm22}
    );
    spheres.push_back(
        {Sphere(
             {13.811053191567627, 0.5247337232151771, -2.6287863275747103},
             0.5247337232151771
         ),
         &sm23}
    );
    spheres.push_back(
        {Sphere(
             {14.472170061239545, 0.23256730820525706, -2.521272038920702},
             0.23256730820525706
         ),
         &sm24}
    );
    spheres.push_back(
        {Sphere(
             {13.801873884443449, 0.1454718638982912, 10.844388130890259},
             0.1454718638982912
         ),
         &sm25}
    );
    spheres.push_back(
        {Sphere(
             {-9.65327209457024, 0.46395044536413665, 0.36307383915008806},
             0.46395044536413665
         ),
         &sm26}
    );
    spheres.push_back(
        {Sphere(
             {-12.979515427694155, 0.3004947402743538, 5.833987608294471},
             0.3004947402743538
         ),
         &sm27}
    );
    spheres.push_back(
        {Sphere(
             {-13.433591320728459, 0.1566098827432429, -0.8940157685809256},
             0.1566098827432429
         ),
         &sm28}
    );
    spheres.push_back(
        {Sphere(
             {0.9257167989197885, 0.35112927174340364, -0.8629868035133281},
             0.35112927174340364
         ),
         &sm29}
    );
    spheres.push_back(
        {Sphere(
             {-13.505576827427024, 0.3797298155578248, 0.13841360501658428},
             0.3797298155578248
         ),
         &sm30}
    );
    spheres.push_back(
        {Sphere(
             {-10.255039707056696, 0.35230685926682603, 17.08168809049407},
             0.35230685926682603
         ),
         &sm31}
    );
    spheres.push_back(
        {Sphere(
             {-14.208982882339402, 0.37842912493832626, 17.05489855120075},
             0.37842912493832626
         ),
         &sm32}
    );
    spheres.push_back(
        {Sphere(
             {1.7001008837147573, 0.32581231594091914, 15.87174277068235},
             0.32581231594091914
         ),
         &sm33}
    );
    spheres.push_back(
        {Sphere(
             {-6.31943693649329, 0.24218625696304752, 16.96742641128804},
             0.24218625696304752
         ),
         &sm34}
    );
    spheres.push_back(
        {Sphere(
             {14.069287403327877, 0.1333288360465097, 18.878370254696595},
             0.1333288360465097
         ),
         &sm35}
    );
    spheres.push_back(
        {Sphere(
             {-14.185941258661636, 0.3179602090706608, -6.5674775260157725},
             0.3179602090706608
         ),
         &sm36}
    );
    spheres.push_back(
        {Sphere(
             {-11.81500420056124, 0.3809631251697841, 20.724577799029902},
             0.3809631251697841
         ),
         &sm37}
    );
    spheres.push_back(
        {Sphere(
             {8.057576875213918, 0.17866845104172158, 20.7876089489827},
             0.17866845104172158
         ),
         &sm38}
    );
    spheres.push_back(
        {Sphere(
             {10.912339726279495, 0.21383979927317154, -1.537900165143089},
             0.21383979927317154
         ),
         &sm39}
    );
    spheres.push_back(
        {Sphere(
             {-12.702595212499928, 0.2202537440820027, 10.819479788990073},
             0.2202537440820027
         ),
         &sm40}
    );
    spheres.push_back(
        {Sphere(
             {-14.338866856813244, 0.08836281476386772, 20.11262156631155},
             0.08836281476386772
         ),
         &sm41}
    );
    spheres.push_back(
        {Sphere(
             {5.777973897214775, 0.42197021508596255, 8.704915730699362},
             0.42197021508596255
         ),
         &sm42}
    );
    spheres.push_back(
        {Sphere(
             {8.556319331961628, 0.2541371348407079, 15.376349291785736},
             0.2541371348407079
         ),
         &sm43}
    );
    spheres.push_back(
        {Sphere(
             {3.6307114839011234, 0.38665659234816757, 13.277948669987907},
             0.38665659234816757
         ),
         &sm44}
    );
    spheres.push_back(
        {Sphere(
             {10.956921168412425, 0.5197771761376997, 6.164856649200294},
             0.5197771761376997
         ),
         &sm45}
    );
    spheres.push_back(
        {Sphere(
             {-7.604217397130886, 0.5367399451927911, 11.516154631323836},
             0.5367399451927911
         ),
         &sm46}
    );
    spheres.push_back(
        {Sphere(
             {-8.910612120836287, 0.5393877618786422, -7.059426985028789},
             0.5393877618786422
         ),
         &sm47}
    );
    spheres.push_back(
        {Sphere(
             {-6.982022798028892, 0.49232324882672635, 18.21195577602693},
             0.49232324882672635
         ),
         &sm48}
    );
    spheres.push_back(
        {Sphere(
             {1.0736248407468167, 0.35982685442132306, 4.213609044666555},
             0.35982685442132306
         ),
         &sm49}
    );
    spheres.push_back(
        {Sphere(
             {-6.487523020415516, 0.5447344733068116, -6.428624999832243},
             0.5447344733068116
         ),
         &sm50}
    );
    spheres.push_back(
        {Sphere(
             {10.540631082571082, 0.19984863405782993, 16.157533301443472},
             0.19984863405782993
         ),
         &sm51}
    );
    spheres.push_back(
        {Sphere(
             {0.683686519405093, 0.14042750669432724, 10.693702606609964},
             0.14042750669432724
         ),
         &sm52}
    );
    spheres.push_back(
        {Sphere(
             {-0.8187668615940797, 0.32840003932629247, 8.847267771959096},
             0.32840003932629247
         ),
         &sm53}
    );
    spheres.push_back(
        {Sphere(
             {-5.539390497751134, 0.30935831953459186, -3.754936480315058},
             0.30935831953459186
         ),
         &sm54}
    );
    spheres.push_back(
        {Sphere(
             {5.878437102344591, 0.3764461196909231, 2.2889169753802854},
             0.3764461196909231
         ),
         &sm55}
    );
    spheres.push_back(
        {Sphere(
             {-12.892246305787914, 0.4926654193498992, 13.108657854026788},
             0.4926654193498992
         ),
         &sm56}
    );
    spheres.push_back(
        {Sphere(
             {-2.6649534737324885, 0.052108569635944194, -6.7893051663229995},
             0.052108569635944194
         ),
         &sm57}
    );
    spheres.push_back(
        {Sphere(
             {2.5255352431511398, 0.31990702617280126, 13.939425513276024},
             0.31990702617280126
         ),
         &sm58}
    );
    spheres.push_back(
        {Sphere(
             {9.726213116178538, 0.47738732491229474, 19.806891822317958},
             0.47738732491229474
         ),
         &sm59}
    );
    spheres.push_back(
        {Sphere(
             {1.997631415211714, 0.37858330734855494, 18.9220613483432},
             0.37858330734855494
         ),
         &sm60}
    );
    spheres.push_back(
        {Sphere(
             {0.7150422964595933, 0.43982369414381456, -2.4282345311621505},
             0.43982369414381456
         ),
         &sm61}
    );
    spheres.push_back(
        {Sphere(
             {0.0906559638153599, 0.46063325345681316, 17.014392716978907},
             0.46063325345681316
         ),
         &sm62}
    );
    spheres.push_back(
        {Sphere(
             {-9.575580697649357, 0.11263584234617492, -2.251752015712877},
             0.11263584234617492
         ),
         &sm63}
    );
    spheres.push_back(
        {Sphere(
             {1.5559608710886152, 0.4533416305045711, -5.9757613357784685},
             0.4533416305045711
         ),
         &sm64}
    );
    spheres.push_back(
        {Sphere(
             {3.113782815709815, 0.17877447405299735, 19.345671689608803},
             0.17877447405299735
         ),
         &sm65}
    );
    spheres.push_back(
        {Sphere(
             {-10.5431992388387, 0.32058745766369284, 21.039594440805057},
             0.32058745766369284
         ),
         &sm66}
    );
    spheres.push_back(
        {Sphere(
             {-3.4284680766352977, 0.15707727321504705, -4.466731262396991},
             0.15707727321504705
         ),
         &sm67}
    );
    spheres.push_back(
        {Sphere(
             {-7.219401928903494, 0.269137820318552, -5.059921955971256},
             0.269137820318552
         ),
         &sm68}
    );
    spheres.push_back(
        {Sphere(
             {11.982621419395265, 0.24107571863177696, -7.362727544288487},
             0.24107571863177696
         ),
         &sm69}
    );
    spheres.push_back(
        {Sphere(
             {13.622952027925631, 0.06913561557231308, -4.98358331069932},
             0.06913561557231308
         ),
         &sm70}
    );
    spheres.push_back(
        {Sphere(
             {-9.027901246533474, 0.3120721811600633, -5.730231690776723},
             0.3120721811600633
         ),
         &sm71}
    );
    spheres.push_back(
        {Sphere(
             {13.855964008826632, 0.44777700651475394, 14.575087295935948},
             0.44777700651475394
         ),
         &sm72}
    );
    spheres.push_back(
        {Sphere(
             {13.915306361202525, 0.15453939427881697, -3.6678986200234687},
             0.15453939427881697
         ),
         &sm73}
    );
    spheres.push_back(
        {Sphere(
             {-13.140478526457361, 0.17428915314446602, -5.015236327713396},
             0.17428915314446602
         ),
         &sm74}
    );
    spheres.push_back(
        {Sphere(
             {7.0629418190473, 0.12260795505351209, 3.4645845750684803},
             0.12260795505351209
         ),
         &sm75}
    );
    spheres.push_back(
        {Sphere(
             {6.7358043254553195, 0.15680965402121094, 9.66541426113746},
             0.15680965402121094
         ),
         &sm76}
    );
    spheres.push_back(
        {Sphere(
             {-7.84436907117923, 0.5318080656347987, -1.197076163075458},
             0.5318080656347987
         ),
         &sm77}
    );
    spheres.push_back(
        {Sphere(
             {3.6616934627368494, 0.09000717954572736, 18.37224599725431},
             0.09000717954572736
         ),
         &sm78}
    );
    spheres.push_back(
        {Sphere(
             {-5.082476124001957, 0.4379057353867454, 8.51084965020003},
             0.4379057353867454
         ),
         &sm79}
    );
    spheres.push_back(
        {Sphere(
             {11.904614853522709, 0.08948292412775473, -7.079564494783973},
             0.08948292412775473
         ),
         &sm80}
    );
    spheres.push_back(
        {Sphere(
             {-7.662185683992271, 0.17142725819578364, 22.1304367954485},
             0.17142725819578364
         ),
         &sm81}
    );
    spheres.push_back(
        {Sphere(
             {7.434888271768184, 0.4376885934594988, 15.795822298494159},
             0.4376885934594988
         ),
         &sm82}
    );
    spheres.push_back(
        {Sphere(
             {4.127280896305592, 0.15228398234864227, 13.325422706741275},
             0.15228398234864227
         ),
         &sm83}
    );
    spheres.push_back(
        {Sphere(
             {-14.779664423437833, 0.30431020080837107, 4.3521898733275},
             0.30431020080837107
         ),
         &sm84}
    );
    spheres.push_back(
        {Sphere(
             {2.0095645276079175, 0.386229609969245, 8.024610919906578},
             0.386229609969245
         ),
         &sm85}
    );
    spheres.push_back(
        {Sphere(
             {12.29895000579733, 0.23583203017686166, 6.326593226777604},
             0.23583203017686166
         ),
         &sm86}
    );
    spheres.push_back(
        {Sphere(
             {3.1608131105104498, 0.3908295463262292, 11.91066835820002},
             0.3908295463262292
         ),
         &sm87}
    );
    spheres.push_back(
        {Sphere(
             {-4.925285942141439, 0.24213281277540538, 18.908584944360143},
             0.24213281277540538
         ),
         &sm88}
    );
    spheres.push_back(
        {Sphere(
             {10.709799939162757, 0.07581963590556563, 1.1739084797254442},
             0.07581963590556563
         ),
         &sm89}
    );
    spheres.push_back(
        {Sphere(
             {-2.594539815035999, 0.4710803254235333, 10.842973987473279},
             0.4710803254235333
         ),
         &sm90}
    );
    spheres.push_back(
        {Sphere(
             {9.552323823837305, 0.4306893751249257, 9.534995222400902},
             0.4306893751249257
         ),
         &sm91}
    );
    spheres.push_back(
        {Sphere(
             {-14.046263507962006, 0.3857985149110834, 4.664586338645892},
             0.3857985149110834
         ),
         &sm92}
    );
    spheres.push_back(
        {Sphere(
             {-5.584926479345443, 0.3304135231723334, 11.814804942465834},
             0.3304135231723334
         ),
         &sm93}
    );
    spheres.push_back(
        {Sphere(
             {11.12099812281646, 0.10004359564883848, 11.26679473160613},
             0.10004359564883848
         ),
         &sm94}
    );
    spheres.push_back(
        {Sphere(
             {-0.7994072239622394, 0.400697548096356, 10.816458963819375},
             0.400697548096356
         ),
         &sm95}
    );
    spheres.push_back(
        {Sphere(
             {6.102506750327102, 0.15573142841928428, 6.1910525650235275},
             0.15573142841928428
         ),
         &sm96}
    );
    spheres.push_back(
        {Sphere(
             {4.786755593563157, 0.4267558096566818, -1.3732070206519893},
             0.4267558096566818
         ),
         &sm97}
    );
    spheres.push_back(
        {Sphere(
             {-6.175172076264847, 0.3636185865100137, 1.0382607578984064},
             0.3636185865100137
         ),
         &sm98}
    );
    spheres.push_back(
        {Sphere(
             {11.975091474653816, 0.4015237402636844, -1.4245013025806799},
             0.4015237402636844
         ),
         &sm99}
    );
    spheres.push_back(
        {Sphere(
             {2.6097359081931017, 0.18263698731197575, 2.245826736779854},
             0.18263698731197575
         ),
         &sm100}
    );
    spheres.push_back(
        {Sphere(
             {-0.051392477943370096, 0.34226494670951363, 9.578216627276689},
             0.34226494670951363
         ),
         &sm101}
    );
    spheres.push_back(
        {Sphere(
             {-7.18268189875409, 0.27042065583385455, 9.006883675491899},
             0.27042065583385455
         ),
         &sm102}
    );
    spheres.push_back(
        {Sphere(
             {10.68744019239795, 0.40118028382918813, 3.8499896673980807},
             0.40118028382918813
         ),
         &sm103}
    );
    spheres.push_back(
        {Sphere(
             {-8.18579330609834, 0.4160343620673077, 5.785345752850722},
             0.4160343620673077
         ),
         &sm104}
    );
    spheres.push_back(
        {Sphere(
             {2.0455812948450713, 0.12461035751156309, -5.363009330826001},
             0.12461035751156309
         ),
         &sm105}
    );
    spheres.push_back(
        {Sphere(
             {10.276190218215316, 0.19982392793981285, 16.17440400579925},
             0.19982392793981285
         ),
         &sm106}
    );
    spheres.push_back(
        {Sphere(
             {0.413026762626302, 0.3494558832179224, 0.8140399029666838},
             0.3494558832179224
         ),
         &sm107}
    );
    spheres.push_back(
        {Sphere(
             {-12.3693049534563, 0.22293658799940086, 4.88151754300857},
             0.22293658799940086
         ),
         &sm108}
    );
    spheres.push_back(
        {Sphere(
             {0.8932899320216023, 0.3542173623603344, 3.929779436344557},
             0.3542173623603344
         ),
         &sm109}
    );
    spheres.push_back(
        {Sphere(
             {-11.083819591296123, 0.05462190153966824, 14.189773828908358},
             0.05462190153966824
         ),
         &sm110}
    );
    spheres.push_back(
        {Sphere(
             {-7.810408281265918, 0.4314442666350712, -2.6587738994279553},
             0.4314442666350712
         ),
         &sm111}
    );
    spheres.push_back(
        {Sphere(
             {1.9320200753784036, 0.5122964248478347, 3.193438729660256},
             0.5122964248478347
         ),
         &sm112}
    );
    spheres.push_back(
        {Sphere(
             {-4.647390616621422, 0.2760056781418059, 3.101753438794189},
             0.2760056781418059
         ),
         &sm113}
    );
    spheres.push_back(
        {Sphere(
             {-7.002932231026562, 0.5181912863362661, 10.30652694337126},
             0.5181912863362661
         ),
         &sm114}
    );
    spheres.push_back(
        {Sphere(
             {2.233347259143194, 0.3416767803657846, -5.120923411121479},
             0.3416767803657846
         ),
         &sm115}
    );
    spheres.push_back(
        {Sphere(
             {-12.086389616106814, 0.4945905302403876, 3.8429660684946736},
             0.4945905302403876
         ),
         &sm116}
    );
    spheres.push_back(
        {Sphere(
             {12.066001760832023, 0.4557422904944109, 18.838464039952584},
             0.4557422904944109
         ),
         &sm117}
    );
    spheres.push_back(
        {Sphere(
             {-1.2889951501262846, 0.15610639543047627, 8.113355811412893},
             0.15610639543047627
         ),
         &sm118}
    );
    spheres.push_back(
        {Sphere(
             {7.373773737067349, 0.23302982604372857, 13.611521358267748},
             0.23302982604372857
         ),
         &sm119}
    );
    spheres.push_back(
        {Sphere(
             {2.052561258807362, 0.5015233526651394, 14.087661099459908},
             0.5015233526651394
         ),
         &sm120}
    );
    spheres.push_back(
        {Sphere(
             {-2.7124697026281694, 0.48722513890253194, 4.463864470092222},
             0.48722513890253194
         ),
         &sm121}
    );
    spheres.push_back(
        {Sphere(
             {1.6814127899427689, 0.21971922397042049, 10.542224009999885},
             0.21971922397042049
         ),
         &sm122}
    );
    spheres.push_back(
        {Sphere(
             {12.449973672133055, 0.21585213220569927, 11.237306340983174},
             0.21585213220569927
         ),
         &sm123}
    );
    spheres.push_back(
        {Sphere(
             {9.427825803879248, 0.24421393117207657, 13.77381628194944},
             0.24421393117207657
         ),
         &sm124}
    );
    spheres.push_back(
        {Sphere(
             {3.5205106653202805, 0.141474528938057, 8.43287208738186},
             0.141474528938057
         ),
         &sm125}
    );
    spheres.push_back(
        {Sphere(
             {7.29612867804688, 0.12265176073388924, 11.245691699587773},
             0.12265176073388924
         ),
         &sm126}
    );
    spheres.push_back(
        {Sphere(
             {-1.595196374151918, 0.3362359741053618, 9.120062532977716},
             0.3362359741053618
         ),
         &sm127}
    );
    spheres.push_back(
        {Sphere(
             {8.087530611965448, 0.39238065610005707, -5.859191382890989},
             0.39238065610005707
         ),
         &sm128}
    );
    spheres.push_back(
        {Sphere(
             {-10.74947208039777, 0.18396845480410745, -5.081731712303151},
             0.18396845480410745
         ),
         &sm129}
    );
    spheres.push_back(
        {Sphere(
             {-14.34635166461305, 0.4415622861255179, 9.240304564801527},
             0.4415622861255179
         ),
         &sm130}
    );
    spheres.push_back(
        {Sphere(
             {-1.2808773019707829, 0.29507991182461646, 13.969138459884107},
             0.29507991182461646
         ),
         &sm131}
    );
    spheres.push_back(
        {Sphere(
             {8.350414162669395, 0.4841713956268799, 5.170841927881995},
             0.4841713956268799
         ),
         &sm132}
    );
    spheres.push_back(
        {Sphere(
             {5.7794732861565805, 0.3554473745500554, 19.86746315212376},
             0.3554473745500554
         ),
         &sm133}
    );
    spheres.push_back(
        {Sphere(
             {11.654114181535757, 0.409639919328334, -7.340931587916226},
             0.409639919328334
         ),
         &sm134}
    );
    spheres.push_back(
        {Sphere(
             {9.784877016169396, 0.28599302369626717, -4.60393042062104},
             0.28599302369626717
         ),
         &sm135}
    );
    spheres.push_back(
        {Sphere(
             {-3.7892856602795, 0.14998402730698568, -5.555316145746939},
             0.14998402730698568
         ),
         &sm136}
    );
    spheres.push_back(
        {Sphere(
             {-13.305213233713452, 0.12654817929346818, 15.584343839828279},
             0.12654817929346818
         ),
         &sm137}
    );
    spheres.push_back(
        {Sphere(
             {7.532106394517822, 0.43548454052462426, -1.3139443848168266},
             0.43548454052462426
         ),
         &sm138}
    );
    spheres.push_back(
        {Sphere(
             {-11.68593161024854, 0.5459668755076759, 5.35116995435526},
             0.5459668755076759
         ),
         &sm139}
    );
    spheres.push_back(
        {Sphere(
             {6.949724622050297, 0.25058243023648874, 4.945578437399357},
             0.25058243023648874
         ),
         &sm140}
    );
    spheres.push_back(
        {Sphere(
             {-10.243266314070857, 0.4642484313579129, 8.477444922492394},
             0.4642484313579129
         ),
         &sm141}
    );
    spheres.push_back(
        {Sphere(
             {8.427245331994374, 0.2741518695793637, 13.777291437128614},
             0.2741518695793637
         ),
         &sm142}
    );
    spheres.push_back(
        {Sphere(
             {11.438386139213296, 0.19158163367887643, 8.708807464143383},
             0.19158163367887643
         ),
         &sm143}
    );
    spheres.push_back(
        {Sphere(
             {11.218594752185751, 0.3145195427653162, 8.424355962764436},
             0.3145195427653162
         ),
         &sm144}
    );
    spheres.push_back(
        {Sphere(
             {-7.58588667220815, 0.06711006077000815, 4.331682541787632},
             0.06711006077000815
         ),
         &sm145}
    );
    spheres.push_back(
        {Sphere(
             {-8.706238870791964, 0.08092414994252324, 14.146533872375397},
             0.08092414994252324
         ),
         &sm146}
    );
    spheres.push_back(
        {Sphere(
             {-4.13224788649649, 0.4679260452854887, -5.162447729345127},
             0.4679260452854887
         ),
         &sm147}
    );
    spheres.push_back(
        {Sphere(
             {-11.084429208186677, 0.32643583898189554, 16.50416678711981},
             0.32643583898189554
         ),
         &sm148}
    );
    spheres.push_back(
        {Sphere(
             {7.960428147197906, 0.5258214579160585, 8.094253633464541},
             0.5258214579160585
         ),
         &sm149}
    );
    spheres.push_back(
        {Sphere(
             {-3.2440611519042566, 0.5218681575407492, 6.865061199156422},
             0.5218681575407492
         ),
         &sm150}
    );
    spheres.push_back(
        {Sphere(
             {11.541819480581609, 0.4933120209785479, 1.3992018943678097},
             0.4933120209785479
         ),
         &sm151}
    );
    spheres.push_back(
        {Sphere(
             {-12.743589433724594, 0.34261539474476693, 20.007778088305155},
             0.34261539474476693
         ),
         &sm152}
    );
    spheres.push_back(
        {Sphere(
             {9.5134749872019, 0.3747492781771009, 10.837951680931301},
             0.3747492781771009
         ),
         &sm153}
    );
    spheres.push_back(
        {Sphere(
             {10.619084197633693, 0.406436771081356, -3.411370963494747},
             0.406436771081356
         ),
         &sm154}
    );
    spheres.push_back(
        {Sphere(
             {12.21363685709731, 0.4910815147396458, 5.682899290864599},
             0.4910815147396458
         ),
         &sm155}
    );
    spheres.push_back(
        {Sphere(
             {-9.68109235344516, 0.12274719281927872, 20.227179375509472},
             0.12274719281927872
         ),
         &sm156}
    );
    spheres.push_back(
        {Sphere(
             {-11.033166403768615, 0.4861661565508266, 9.424274918942292},
             0.4861661565508266
         ),
         &sm157}
    );
    spheres.push_back(
        {Sphere(
             {0.4980461430611509, 0.11950337885554958, 5.8928388270731915},
             0.11950337885554958
         ),
         &sm158}
    );
    spheres.push_back(
        {Sphere(
             {-12.937353945400886, 0.5025206198279828, -1.4250492474598895},
             0.5025206198279828
         ),
         &sm159}
    );
    spheres.push_back(
        {Sphere(
             {0.6961963187637448, 0.4606823239844361, -4.595978851379382},
             0.4606823239844361
         ),
         &sm160}
    );
    spheres.push_back(
        {Sphere(
             {13.338342796516379, 0.49016557221270757, 21.212108273281988},
             0.49016557221270757
         ),
         &sm161}
    );
    spheres.push_back(
        {Sphere(
             {-8.992794310874949, 0.18042141793720234, -4.000640711954514},
             0.18042141793720234
         ),
         &sm162}
    );
    spheres.push_back(
        {Sphere(
             {-11.214044695926493, 0.25577111601874636, 20.406228247346323},
             0.25577111601874636
         ),
         &sm163}
    );
    spheres.push_back(
        {Sphere(
             {10.881317719695865, 0.18497315863011699, 21.78709461724442},
             0.18497315863011699
         ),
         &sm164}
    );
    spheres.push_back(
        {Sphere(
             {-0.928505303190768, 0.20722333304781587, -1.7193456356968202},
             0.20722333304781587
         ),
         &sm165}
    );
    spheres.push_back(
        {Sphere(
             {-5.171075326887923, 0.46828477040202715, 13.446892289295029},
             0.46828477040202715
         ),
         &sm166}
    );
    spheres.push_back(
        {Sphere(
             {-11.84067239221648, 0.3166153362327121, 0.04308696826032854},
             0.3166153362327121
         ),
         &sm167}
    );
    spheres.push_back(
        {Sphere(
             {-2.453861823100924, 0.08456507058161729, 8.052169433791427},
             0.08456507058161729
         ),
         &sm168}
    );
    spheres.push_back(
        {Sphere(
             {-2.6085413542639255, 0.16436152886025301, 17.300104272202343},
             0.16436152886025301
         ),
         &sm169}
    );
    spheres.push_back(
        {Sphere(
             {-0.4570892521550398, 0.4118674746444396, -6.524536608570292},
             0.4118674746444396
         ),
         &sm170}
    );
    spheres.push_back(
        {Sphere(
             {14.830556040782632, 0.361433341322143, 13.368392596392123},
             0.361433341322143
         ),
         &sm171}
    );
    spheres.push_back(
        {Sphere(
             {8.832182690574982, 0.1367892805827156, 17.034073382825778},
             0.1367892805827156
         ),
         &sm172}
    );
    spheres.push_back(
        {Sphere(
             {10.94800020037598, 0.15630334492620968, -3.6108156389008483},
             0.15630334492620968
         ),
         &sm173}
    );
    spheres.push_back(
        {Sphere(
             {-14.606874308671983, 0.4919871355113143, 21.040421127347383},
             0.4919871355113143
         ),
         &sm174}
    );
    spheres.push_back(
        {Sphere(
             {-13.877533801105482, 0.480926893158805, -6.029444967408844},
             0.480926893158805
         ),
         &sm175}
    );
    spheres.push_back(
        {Sphere(
             {-1.2166972364135074, 0.4636742929798572, 14.368575975140761},
             0.4636742929798572
         ),
         &sm176}
    );
    spheres.push_back(
        {Sphere(
             {3.340046232457265, 0.46945486917742, 8.921756816232783},
             0.46945486917742
         ),
         &sm177}
    );
    spheres.push_back(
        {Sphere(
             {-4.194899432189498, 0.31202412653116846, 9.304737598370341},
             0.31202412653116846
         ),
         &sm178}
    );
    spheres.push_back(
        {Sphere(
             {10.8216183521041, 0.2089791852040036, 9.72647480288482},
             0.2089791852040036
         ),
         &sm179}
    );
    spheres.push_back(
        {Sphere(
             {-1.4356254954110153, 0.4477320731110071, 4.600041279632066},
             0.4477320731110071
         ),
         &sm180}
    );
    spheres.push_back(
        {Sphere(
             {12.655093129957297, 0.1932290114722739, 21.081331637673323},
             0.1932290114722739
         ),
         &sm181}
    );
    spheres.push_back(
        {Sphere(
             {-11.61571509984038, 0.5103404317030097, -0.24568922627311007},
             0.5103404317030097
         ),
         &sm182}
    );
    spheres.push_back(
        {Sphere(
             {-10.665421908967776, 0.36429748120399874, 0.6853996713634132},
             0.36429748120399874
         ),
         &sm183}
    );
    spheres.push_back(
        {Sphere(
             {3.775949910892553, 0.5077193501817838, -2.2817251218002434},
             0.5077193501817838
         ),
         &sm184}
    );
    spheres.push_back(
        {Sphere(
             {6.123756810786908, 0.26722509008964795, 9.429740830518494},
             0.26722509008964795
         ),
         &sm185}
    );
    spheres.push_back(
        {Sphere(
             {-7.640305627194996, 0.4811031999629343, -5.934544695232527},
             0.4811031999629343
         ),
         &sm186}
    );
    spheres.push_back(
        {Sphere(
             {0.04542782605791906, 0.2509099035379766, 8.085622626004117},
             0.2509099035379766
         ),
         &sm187}
    );
    spheres.push_back(
        {Sphere(
             {-9.284164757561516, 0.090400535749127, 3.017072079901382},
             0.090400535749127
         ),
         &sm188}
    );
    spheres.push_back(
        {Sphere(
             {-2.1724764751507095, 0.5401601319975144, 20.691136323172124},
             0.5401601319975144
         ),
         &sm189}
    );
    spheres.push_back(
        {Sphere(
             {-13.150370779839855, 0.09260854873292608, 7.844368419335218},
             0.09260854873292608
         ),
         &sm190}
    );
    spheres.push_back(
        {Sphere(
             {-3.4522178180769743, 0.2804145914523105, 7.854997938829101},
             0.2804145914523105
         ),
         &sm191}
    );
    spheres.push_back(
        {Sphere(
             {-4.990537376057279, 0.37781058275250917, 7.487690530672074},
             0.37781058275250917
         ),
         &sm192}
    );
    spheres.push_back(
        {Sphere(
             {-11.677175921986438, 0.3322371454728845, 19.678350939790622},
             0.3322371454728845
         ),
         &sm193}
    );
    spheres.push_back(
        {Sphere(
             {-9.08150101906239, 0.4034505323011333, 21.960088204664913},
             0.4034505323011333
         ),
         &sm194}
    );
    spheres.push_back(
        {Sphere(
             {-7.84528634627883, 0.51895651914742, -0.7874963826068813},
             0.51895651914742
         ),
         &sm195}
    );
    spheres.push_back(
        {Sphere(
             {0.9260651403732467, 0.5015694672575964, 9.039827124312541},
             0.5015694672575964
         ),
         &sm196}
    );
    spheres.push_back(
        {Sphere(
             {12.692386184222094, 0.12356098191263688, -5.576182968346583},
             0.12356098191263688
         ),
         &sm197}
    );
    spheres.push_back(
        {Sphere(
             {-7.516073919277824, 0.11862232427899073, 14.846038855834909},
             0.11862232427899073
         ),
         &sm198}
    );
    spheres.push_back(
        {Sphere(
             {-5.670534892106236, 0.18398844684226545, 21.704091401834226},
             0.18398844684226545
         ),
         &sm199}
    );
    spheres.push_back(
        {Sphere(
             {-7.633294857790269, 0.14823212895342408, 1.5638877887094953},
             0.14823212895342408
         ),
         &sm200}
    );
    spheres.push_back(
        {Sphere(
             {-1.337600496711005, 0.08720246993108312, -5.809581707332496},
             0.08720246993108312
         ),
         &sm201}
    );
    spheres.push_back(
        {Sphere(
             {-5.986728497154365, 0.1330522070250975, 17.71666232413146},
             0.1330522070250975
         ),
         &sm202}
    );
    spheres.push_back(
        {Sphere(
             {-1.4392473083783681, 0.3213144263066278, 14.837940148075571},
             0.3213144263066278
         ),
         &sm203}
    );
    spheres.push_back(
        {Sphere(
             {13.156338800743107, 0.48436751588628424, 16.23495950078801},
             0.48436751588628424
         ),
         &sm204}
    );
    spheres.push_back(
        {Sphere(
             {-4.734291573692692, 0.0621297498839299, 12.764988618504017},
             0.0621297498839299
         ),
         &sm205}
    );
    spheres.push_back(
        {Sphere(
             {-6.080387268966694, 0.4887728303661033, 20.990523369654365},
             0.4887728303661033
         ),
         &sm206}
    );
    spheres.push_back(
        {Sphere(
             {13.684359404048607, 0.4114929554480365, 8.566004226327447},
             0.4114929554480365
         ),
         &sm207}
    );
    spheres.push_back(
        {Sphere(
             {3.7417781331622813, 0.20055242634039772, 13.630066174314656},
             0.20055242634039772
         ),
         &sm208}
    );
    spheres.push_back(
        {Sphere(
             {4.716354532158174, 0.2794549283860674, -5.380346716849874},
             0.2794549283860674
         ),
         &sm209}
    );
    spheres.push_back(
        {Sphere(
             {-9.83609400383575, 0.1740894863961452, 15.8532661130657},
             0.1740894863961452
         ),
         &sm210}
    );
    spheres.push_back(
        {Sphere(
             {11.95459855032949, 0.30749810147101087, 2.389815013000865},
             0.30749810147101087
         ),
         &sm211}
    );
    spheres.push_back(
        {Sphere(
             {3.401979205912429, 0.13681826601438457, 0.08556723784983511},
             0.13681826601438457
         ),
         &sm212}
    );
    spheres.push_back(
        {Sphere(
             {2.2581250591242146, 0.5366493619387871, 13.418414908618683},
             0.5366493619387871
         ),
         &sm213}
    );
    spheres.push_back(
        {Sphere(
             {-8.378978347468768, 0.18241109659881483, -6.532847719171507},
             0.18241109659881483
         ),
         &sm214}
    );
    spheres.push_back(
        {Sphere(
             {13.249537159462086, 0.2790491899777024, 19.013215870108652},
             0.2790491899777024
         ),
         &sm215}
    );
    spheres.push_back(
        {Sphere(
             {13.815188075234566, 0.33891786110487937, 20.517323602911663},
             0.33891786110487937
         ),
         &sm216}
    );
    spheres.push_back(
        {Sphere(
             {-1.0647981086924538, 0.4904814681978468, 21.30096438142691},
             0.4904814681978468
         ),
         &sm217}
    );
    spheres.push_back(
        {Sphere(
             {9.925730123234711, 0.12273411253933493, 17.395926605505537},
             0.12273411253933493
         ),
         &sm218}
    );
    spheres.push_back(
        {Sphere(
             {5.764265270994951, 0.19979579697573985, 17.392392467941043},
             0.19979579697573985
         ),
         &sm219}
    );
    spheres.push_back(
        {Sphere(
             {14.921845143256288, 0.07106429813546179, 16.399490314029418},
             0.07106429813546179
         ),
         &sm220}
    );
    spheres.push_back(
        {Sphere(
             {-9.170325677073581, 0.09508236956528894, 13.878612260334144},
             0.09508236956528894
         ),
         &sm221}
    );
    spheres.push_back(
        {Sphere(
             {2.783825111709028, 0.32811805923964404, 3.7199562042433154},
             0.32811805923964404
         ),
         &sm222}
    );
    spheres.push_back(
        {Sphere(
             {-4.937557655394292, 0.11292910014313597, 3.574938383938802},
             0.11292910014313597
         ),
         &sm223}
    );
    spheres.push_back(
        {Sphere(
             {3.778838078132516, 0.4561810309643057, 10.182631147544214},
             0.4561810309643057
         ),
         &sm224}
    );
    spheres.push_back(
        {Sphere(
             {13.155416886692663, 0.4138266986967274, 8.26154401874299},
             0.4138266986967274
         ),
         &sm225}
    );
    spheres.push_back(
        {Sphere(
             {4.610326819748163, 0.05002622096590216, -3.871783497706107},
             0.05002622096590216
         ),
         &sm226}
    );
    spheres.push_back(
        {Sphere(
             {6.410130766313074, 0.05319043492937588, -3.7129364159262646},
             0.05319043492937588
         ),
         &sm227}
    );
    spheres.push_back(
        {Sphere(
             {8.23254439018261, 0.16169745429236682, -1.6246992639828282},
             0.16169745429236682
         ),
         &sm228}
    );
    spheres.push_back(
        {Sphere(
             {-3.3160118249420467, 0.2814509780751794, -2.5959762652663017},
             0.2814509780751794
         ),
         &sm229}
    );
    spheres.push_back(
        {Sphere(
             {9.911517122365886, 0.30858509797951406, 2.2871557943657255},
             0.30858509797951406
         ),
         &sm230}
    );
    spheres.push_back(
        {Sphere(
             {13.980564548907473, 0.1790496518957898, 20.347924112486435},
             0.1790496518957898
         ),
         &sm231}
    );
    spheres.push_back(
        {Sphere(
             {0.494727023817207, 0.11249618135818863, -3.5774398324532575},
             0.11249618135818863
         ),
         &sm232}
    );
    spheres.push_back(
        {Sphere(
             {-4.725310437032363, 0.4666121289388747, -6.830645756439212},
             0.4666121289388747
         ),
         &sm233}
    );
    spheres.push_back(
        {Sphere(
             {-5.09336699189368, 0.5443821443033743, -2.2091357900935},
             0.5443821443033743
         ),
         &sm234}
    );
    spheres.push_back(
        {Sphere(
             {6.659957798550224, 0.4032426517761027, 21.97383664347477},
             0.4032426517761027
         ),
         &sm235}
    );
    spheres.push_back(
        {Sphere(
             {-12.781624664459997, 0.11996601512136164, 0.2875849239459969},
             0.11996601512136164
         ),
         &sm236}
    );
    spheres.push_back(
        {Sphere(
             {13.444974086464647, 0.3913550729604985, 0.1271622546291855},
             0.3913550729604985
         ),
         &sm237}
    );
    spheres.push_back(
        {Sphere(
             {-14.005745347367395, 0.19392339634849803, -4.4681584462377275},
             0.19392339634849803
         ),
         &sm238}
    );
    spheres.push_back(
        {Sphere(
             {-1.451331772585272, 0.5220697365215823, -5.169248815924041},
             0.5220697365215823
         ),
         &sm239}
    );
    spheres.push_back(
        {Sphere(
             {10.64016997927693, 0.4666001079085971, 15.905176274560706},
             0.4666001079085971
         ),
         &sm240}
    );
    spheres.push_back(
        {Sphere(
             {12.437661025892048, 0.45105955426304317, -6.782544816477831},
             0.45105955426304317
         ),
         &sm241}
    );
    spheres.push_back(
        {Sphere(
             {-7.081490111703388, 0.33581873660676037, 22.23591964790837},
             0.33581873660676037
         ),
         &sm242}
    );
    spheres.push_back(
        {Sphere(
             {-0.4265065556459149, 0.19356248525364245, 4.4613033089852685},
             0.19356248525364245
         ),
         &sm243}
    );
    spheres.push_back(
        {Sphere(
             {3.3455134405519438, 0.17406645815659066, 8.495202186601228},
             0.17406645815659066
         ),
         &sm244}
    );
    spheres.push_back(
        {Sphere(
             {-14.154880723160337, 0.36224722456759784, -1.1147882920538796},
             0.36224722456759784
         ),
         &sm245}
    );
    spheres.push_back(
        {Sphere(
             {-8.16972430610398, 0.37452843983047457, -3.057294554934467},
             0.37452843983047457
         ),
         &sm246}
    );
    spheres.push_back(
        {Sphere(
             {-9.668196550534082, 0.5042067180170026, -0.8874314266636762},
             0.5042067180170026
         ),
         &sm247}
    );
    spheres.push_back(
        {Sphere(
             {-4.231401953492885, 0.43167611810596324, 6.21136747200587},
             0.43167611810596324
         ),
         &sm248}
    );
    spheres.push_back(
        {Sphere(
             {2.865869630399196, 0.37712831767620253, 8.114443321037532},
             0.37712831767620253
         ),
         &sm249}
    );
    spheres.push_back(
        {Sphere(
             {6.006142336108834, 0.34501317169560136, 10.584680875170157},
             0.34501317169560136
         ),
         &sm250}
    );
    spheres.push_back(
        {Sphere(
             {-13.806157672825757, 0.06516825866404354, 1.172206769056917},
             0.06516825866404354
         ),
         &sm251}
    );
    spheres.push_back(
        {Sphere(
             {8.793712313671112, 0.17407314229497084, -5.764843914376119},
             0.17407314229497084
         ),
         &sm252}
    );
    spheres.push_back(
        {Sphere(
             {8.826817698030272, 0.1842306612886998, 5.099744494721616},
             0.1842306612886998
         ),
         &sm253}
    );
    spheres.push_back(
        {Sphere(
             {4.542780782943275, 0.4308192440986802, 2.4729182387660202},
             0.4308192440986802
         ),
         &sm254}
    );
    spheres.push_back(
        {Sphere(
             {12.111077065537646, 0.14156459660102666, 8.043713272918446},
             0.14156459660102666
         ),
         &sm255}
    );
    spheres.push_back(
        {Sphere(
             {7.250727400332725, 0.5321325348774899, 9.656192450564308},
             0.5321325348774899
         ),
         &sm256}
    );
    spheres.push_back(
        {Sphere(
             {14.932548860677883, 0.17374133709685757, -7.135299585200205},
             0.17374133709685757
         ),
         &sm257}
    );
    spheres.push_back(
        {Sphere(
             {0.6423251460260655, 0.1406614904709288, -4.499332783866887},
             0.1406614904709288
         ),
         &sm258}
    );
    spheres.push_back(
        {Sphere(
             {-11.492510015755432, 0.32347726810020344, 11.6250444792935},
             0.32347726810020344
         ),
         &sm259}
    );
    spheres.push_back(
        {Sphere(
             {-3.492488028245152, 0.07080689566061889, 13.198289926693299},
             0.07080689566061889
         ),
         &sm260}
    );
    spheres.push_back(
        {Sphere(
             {10.477804335660203, 0.3918164172333876, 20.70405155531864},
             0.3918164172333876
         ),
         &sm261}
    );
    spheres.push_back(
        {Sphere(
             {14.687564190124167, 0.43010645337101716, 19.076495108714035},
             0.43010645337101716
         ),
         &sm262}
    );
    spheres.push_back(
        {Sphere(
             {-6.742337064185239, 0.0907446035595842, 11.671970211513994},
             0.0907446035595842
         ),
         &sm263}
    );
    spheres.push_back(
        {Sphere(
             {8.298379781468057, 0.24640885630474757, -3.858953010330306},
             0.24640885630474757
         ),
         &sm264}
    );
    spheres.push_back(
        {Sphere(
             {5.725878570449552, 0.4726481631286792, 16.68923656009528},
             0.4726481631286792
         ),
         &sm265}
    );
    spheres.push_back(
        {Sphere(
             {1.875722813566746, 0.2541783339165007, 5.263293573221873},
             0.2541783339165007
         ),
         &sm266}
    );
    spheres.push_back(
        {Sphere(
             {-3.9054064336672205, 0.5132642831312654, 16.358693997424734},
             0.5132642831312654
         ),
         &sm267}
    );
    spheres.push_back(
        {Sphere(
             {-0.6340398950004662, 0.48720265655738254, -5.301727753202369},
             0.48720265655738254
         ),
         &sm268}
    );
    spheres.push_back(
        {Sphere(
             {-6.81532075408678, 0.34234068554775415, 14.141843337435326},
             0.34234068554775415
         ),
         &sm269}
    );
    spheres.push_back(
        {Sphere(
             {10.493643087108364, 0.26573603532101575, 12.901351053817272},
             0.26573603532101575
         ),
         &sm270}
    );
    spheres.push_back(
        {Sphere(
             {-0.5422068120435313, 0.2527000945612213, 7.258242925959358},
             0.2527000945612213
         ),
         &sm271}
    );
    spheres.push_back(
        {Sphere(
             {1.353331969086458, 0.22568765497234955, -3.4678224968725715},
             0.22568765497234955
         ),
         &sm272}
    );
    spheres.push_back(
        {Sphere(
             {-10.473767869456838, 0.40643318939079387, 7.101041952065385},
             0.40643318939079387
         ),
         &sm273}
    );
    spheres.push_back(
        {Sphere(
             {13.641500386196835, 0.07495172548858571, 5.815680400614443},
             0.07495172548858571
         ),
         &sm274}
    );
    spheres.push_back(
        {Sphere(
             {5.149991364133609, 0.2093160299072142, 12.367724894495602},
             0.2093160299072142
         ),
         &sm275}
    );
    spheres.push_back(
        {Sphere(
             {-5.008120553816537, 0.43730280823494583, -1.8504849106371637},
             0.43730280823494583
         ),
         &sm276}
    );
    spheres.push_back(
        {Sphere(
             {7.417631422027103, 0.16561001158619676, 18.3990082065732},
             0.16561001158619676
         ),
         &sm277}
    );
    spheres.push_back(
        {Sphere(
             {-12.79949479132188, 0.4224387157100054, 0.28957462453894944},
             0.4224387157100054
         ),
         &sm278}
    );
    spheres.push_back(
        {Sphere(
             {14.221863560343287, 0.38559885421353174, 20.27959483925131},
             0.38559885421353174
         ),
         &sm279}
    );
    spheres.push_back(
        {Sphere(
             {6.463126122612962, 0.32059018553960833, -5.935362402582682},
             0.32059018553960833
         ),
         &sm280}
    );
    spheres.push_back(
        {Sphere(
             {-2.1386832936149833, 0.5101782089377834, 21.26548796950655},
             0.5101782089377834
         ),
         &sm281}
    );
    spheres.push_back(
        {Sphere(
             {2.707292336053289, 0.14177938161922254, 3.4279879725567515},
             0.14177938161922254
         ),
         &sm282}
    );
    spheres.push_back(
        {Sphere(
             {12.801074645530917, 0.46819116874414723, 2.7965191950659047},
             0.46819116874414723
         ),
         &sm283}
    );
    spheres.push_back(
        {Sphere(
             {-10.408799385884116, 0.4381982082576276, 17.611921004197562},
             0.4381982082576276
         ),
         &sm284}
    );
    spheres.push_back(
        {Sphere(
             {-0.24141904938534609, 0.2701082571505306, 16.641177396300904},
             0.2701082571505306
         ),
         &sm285}
    );
    spheres.push_back(
        {Sphere(
             {3.143007234413645, 0.5491592430963563, -3.936737230649514},
             0.5491592430963563
         ),
         &sm286}
    );
    spheres.push_back(
        {Sphere(
             {-4.542084905192219, 0.3483390725698824, 9.16604066237911},
             0.3483390725698824
         ),
         &sm287}
    );
    spheres.push_back(
        {Sphere(
             {-4.703651573436277, 0.5043372061257939, 12.406052104205685},
             0.5043372061257939
         ),
         &sm288}
    );
    spheres.push_back(
        {Sphere(
             {-7.121142849043602, 0.2831211043488552, 8.9144236082137},
             0.2831211043488552
         ),
         &sm289}
    );
    spheres.push_back(
        {Sphere(
             {1.4510276582812125, 0.057312484995646465, 18.90890603956177},
             0.057312484995646465
         ),
         &sm290}
    );
    spheres.push_back(
        {Sphere(
             {4.42758668457903, 0.42551560071274935, -2.598709347893924},
             0.42551560071274935
         ),
         &sm291}
    );
    spheres.push_back(
        {Sphere(
             {7.446083450728448, 0.22868682426392944, 18.460251032636137},
             0.22868682426392944
         ),
         &sm292}
    );
    spheres.push_back(
        {Sphere(
             {8.593818210838602, 0.36000256450215823, 16.91518979522703},
             0.36000256450215823
         ),
         &sm293}
    );
    spheres.push_back(
        {Sphere(
             {-4.645556417140227, 0.4220041489658429, 17.165184492150345},
             0.4220041489658429
         ),
         &sm294}
    );
    spheres.push_back(
        {Sphere(
             {5.391341047273695, 0.5030511736603959, -0.5142340174098834},
             0.5030511736603959
         ),
         &sm295}
    );
    spheres.push_back(
        {Sphere(
             {13.371719240824518, 0.5184863573069153, 16.013252800509385},
             0.5184863573069153
         ),
         &sm296}
    );
    spheres.push_back(
        {Sphere(
             {0.7815527042101333, 0.5065871489472955, -1.3810003680500316},
             0.5065871489472955
         ),
         &sm297}
    );
    spheres.push_back(
        {Sphere(
             {-10.688028848842693, 0.37843628324156275, 5.336635245162352},
             0.37843628324156275
         ),
         &sm298}
    );
    spheres.push_back(
        {Sphere(
             {14.695032382438086, 0.1175672502438276, 7.977684134827662},
             0.1175672502438276
         ),
         &sm299}
    );

    for (auto& [sphere, mat_ptr] : spheres) {
        os.push_back({&sphere, mat_ptr});
    }
    Plane ground({0, 0, 0}, {0, 1, 0});
    os.push_back({&ground, &ground_mat});

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();
    Image img{{width, height}, {255, 255, 255, 255}};
    PointLight const light(camera.get_position());

    size_t constexpr xi = 700;
    size_t constexpr yi = 350;

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            LightGraphNode
                root{&bg_material, 0, 1, camera.get_line_at(x, y), nullptr};
            root.construct_with_material(os, &bg_material);
            if (y == yi && x == xi) {
                std::cout << root.to_string();
                std::flush(std::cout);
            }
            vec3 const vcol =
                root.calculate_color(camera, light, root.sum_light_intensity());
            img.set_color_at(x, y, to_color(vcol));
        }
    }

    img.save("spheres_example.png");
}
