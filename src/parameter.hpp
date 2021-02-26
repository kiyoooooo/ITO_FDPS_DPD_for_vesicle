#ifndef HPP_PARAMETER
#define HPP_PARAMETER

#include<particle_simulator.hpp>

//const PS::S32 NMOL  = 162000;
//const PS::S32 NMOL  = 2890608; //changed by kiyoshiro "for300"
//const PS::S32 NMOL  = 28795808;//changed by kiyoshiro "for3000"
//const PS::S32 NMOL  =   13679376;//changed by kiyoshiro "for1500"displayed as this by FDPS 
//const PS::S32 NMOL  = 29972768;//changed by kiyoshiro "for3000new"
//const PS::S32 NMOL  = 29996000;//changed by kiyoshiro "for3000_withdelete"
//const PS::S32 NMOL  = 31411200;//changed by kiyoshiro "for3000_withdeletelowlipid"
//const PS::S32 NMOL  = 31315200;//changed by kiyoshiro "for3000_withdeletelowlipid1"
//const PS::S32 NMOL  = 31232000;//changed by kiyoshiro "for3000_withdeletelowlipid2"
//const PS::S32 NMOL  = 31161600;//changed by kiyoshiro "for3000_withdeletelowlipid3"
//const PS::S32 NMOL  = 31517724;//changed by kiyoshiro "for3000_withdeletelowlipid4"
//const PS::S32 NMOL  = 31639324;//changed by kiyoshiro "for3000_withdeletelowlipid5"
//const PS::S32 NMOL  = 31776000;//changed by kiyoshiro "for3000_withdeletelowlipid6"
//const PS::S32 NMOL  = 31920204;//changed by kiyoshiro "for3000_withdeletelowlipid7"
//const PS::S32 NMOL  = 32080204;//changed by kiyoshiro "for3000_withdeletelowlipid8"
//const PS::S32 NMOL  = 32256000;//changed by kiyoshiro "for3000_withdeletelowlipid9"
//const PS::S32 NMOL  = 31080204;//changed by kiyoshiro "for3000_withdeletelowwater0"
//const PS::S32 NMOL  = 30080204;//changed by kiyoshiro "for3000_withdeletelowwater1"
//const PS::S32 NMOL  = 29080204;//changed by kiyoshiro "for3000_withdeletelowwater2"
//const PS::S32 NMOL  = 29739200;//changed by kiyoshiro "for3000_withdelete_opti0"
//const PS::S32 NMOL  = 29244000;//changed by kiyoshiro "for3000_withdelete_opti1"
//const PS::S32 NMOL  = 31187444;//changed by kiyoshiro "for3000_withdelete_opti2"
//const PS::S32 NMOL  = 28848000;//changed by kiyoshiro "for3000_with_hole1"
//const PS::S32 NMOL  = 28536000;//changed by kiyoshiro "for3000_with_hole2"
//const PS::S32 NMOL  = 28464804;//changed by kiyoshiro "for3000_with_minihole0"
//const PS::S32 NMOL  = 28399324;//changed by kiyoshiro "for3000_midium_water0","vesicle_input3000_midium_water_with_onebighole0","vesicle_input3000_midium_water_bigradius0"
//const PS::S32 NMOL  = 40951615;//changed by kiyoshiro "double_vesicle_input3000_midium_water0"
//const PS::S32 NMOL  = 40870919;//changed by kiyoshiro "double_vesicle_input3000_midium_water1"
//const PS::S32 NMOL  = 40870869;//changed by kiyoshiro "double_vesicle_input3000_midium_water4"
//const PS::S32 NMOL  = 40874996;//changed by kiyoshiro "double_vesicle_input3000_midium_water5"
//const PS::S32 NMOL  = 3141832;//changed by kiyoshiro "exp_vesicle4"
const PS::S32 NMOL  = 4406209;//changed by kiyoshiro "exp_double4"
//const PS::S32 NATOM = 4; //what is this?
const PS::S32 NATOM = 1;//chaged by kiyoshiro
const PS::S32 NCHAIN_VECICLE = 4;
const PS::S32 NCHAIN_POLYMER = 80;

const PS::S32 MOL_T = 39480;
const PS::F64 INNER_RADIUS = 5.0;
const PS::F64 INNER_RADIUS2 = INNER_RADIUS*INNER_RADIUS;
const PS::F64 OUTER_RADIUS = 10.0;
const PS::F64 OUTER_RADIUS2 = OUTER_RADIUS*OUTER_RADIUS;

const PS::F64 DZZ = 1.0;

//const PS::F64vec VOLUME = PS::F64vec(30.,30.,60.);
//const PS::F64vec VOLUME = PS::F64vec(45.844,45.844,45.844);//changed by kiyoshiro "for30"
//const PS::F64vec VOLUME = PS::F64vec(98.7695,98.7695,98.7695);//changed by kiyoshiro "for300"
//const PS::F64vec VOLUME = PS::F64vec(212.521,212.521,212.521);//changed by kiyoshiro "for3000"
//const PS::F64vec VOLUME = PS::F64vec(165.824,165.824,165.824);//changed by kiyoshiro "for1500"  
//const PS::F64vec VOLUME = PS::F64vec(215.378,215.378,215.378);//changed by kiyoshiro "for3000new"  
//const PS::F64vec VOLUME = PS::F64vec(215.434,215.434,215.434);//changed by kiyoshiro "for3000withdelete"  
//const PS::F64vec VOLUME = PS::F64vec(218.77,218.77,218.77);//changed by kiyoshiro "for3000withdeletelow"  
//const PS::F64vec VOLUME = PS::F64vec(218.547,218.547,218.547);//changed by kiyoshiro "for3000withdeletelowlipid1"  
//const PS::F64vec VOLUME = PS::F64vec(218.353,218.353,218.353);//changed by kiyoshiro "for3000withdeletelowlipid2"  
//const PS::F64vec VOLUME = PS::F64vec(218.189,218.189,218.189);//changed by kiyoshiro "for3000withdeletelowlipid3"  
//const PS::F64vec VOLUME = PS::F64vec(219.017,219.017,219.017);//changed by kiyoshiro "for3000withdeletelowlipid4"  
//const PS::F64vec VOLUME = PS::F64vec(219.298,219.298,219.298);//changed by kiyoshiro "for3000withdeletelowlipid5"  
//const PS::F64vec VOLUME = PS::F64vec(219.614,219.614,219.614);//changed by kiyoshiro "for3000withdeletelowlipid6"  
//const PS::F64vec VOLUME = PS::F64vec(219.945,219.945,219.945);//changed by kiyoshiro "for3000withdeletelowlipid7"  
//const PS::F64vec VOLUME = PS::F64vec(220.312,220.312,220.312);//changed by kiyoshiro "for3000withdeletelowlipid8"  
//const PS::F64vec VOLUME = PS::F64vec(220.714,220.714,220.714);//changed by kiyoshiro "for3000withdeletelowlipid9"  
//const PS::F64vec VOLUME = PS::F64vec(217.999,217.999,217.999);//changed by kiyoshiro "for3000withdeletelowwater0"  
//const PS::F64vec VOLUME = PS::F64vec(215.635,215.635,215.635);//changed by kiyoshiro "for3000withdeletelowwater1"  
//const PS::F64vec VOLUME = PS::F64vec(213.219,213.219,213.219);//changed by kiyoshiro "for3000withdeletelowwater2"  
//const PS::F64vec VOLUME = PS::F64vec(214.817,214.817,214.817);//changed by kiyoshiro "for3000withdelete_opti0"  
//const PS::F64vec VOLUME = PS::F64vec(213.618,213.618,213.618);//changed by kiyoshiro "for3000withdelete_opti1"  
//const PS::F64vec VOLUME = PS::F64vec(218.249,218.249,218.249);//changed by kiyoshiro "for3000withdelete_opti2"  
//const PS::F64vec VOLUME = PS::F64vec(212.65,212.65,212.65);//changed by kiyoshiro "for3000withhole1"  
//const PS::F64vec VOLUME = PS::F64vec(211.88 ,211.88 ,211.88 );//changed by kiyoshiro "for3000withhole2"  
//const PS::F64vec VOLUME = PS::F64vec(211.704 ,211.704 ,211.704 );//changed by kiyoshiro "for3000minihole0"  
//const PS::F64vec VOLUME = PS::F64vec(211.542 ,211.542 ,211.542 );//changed by kiyoshiro "for3000midium_water0","vesicle_input3000_midium_water_with_onebighole0","vesicle_input3000_midium_water_bigradius0"
//const PS::F64vec VOLUME = PS::F64vec(211.542 ,211.542 ,306.938 );//changed by kiyoshiro "double_vesicle_input3000_midium_water0"
//const PS::F64vec VOLUME = PS::F64vec(211.542 ,211.542 ,304.871 );//changed by kiyoshiro "double_vesicle_input3000_midium_water1"距離を改善した．
//const PS::F64vec VOLUME = PS::F64vec(211.542 ,211.542 ,304.871 );//changed by kiyoshiro "double_vesicle_input3000_midium_water4"距離を改善した．
//const PS::F64vec VOLUME = PS::F64vec(211.542 ,211.542 ,304.901 );//changed by kiyoshiro "double_vesicle_input3000_midium_water5"距離を改善した．
//const PS::F64vec VOLUME = PS::F64vec(101.552 ,101.552 ,101.552 );//changed by kiyoshiro "exp_vesicle4"．
const PS::F64vec VOLUME = PS::F64vec(101.552 ,101.552 ,142.267 );//changed by kiyoshiro "exp_double4"．
const PS::F64vec VOLUME_INV = PS::F64vec(1.0/VOLUME.x, 1.0/VOLUME.y, 1.0/VOLUME.z);
const PS::F64vec VOLUME_HALF = PS::F64vec(0.5*VOLUME.x, 0.5*VOLUME.y, 0.5*VOLUME.z);



const PS::F64 COEF_REP[5][5] =
  {{ 25., 200.,  25.,  25., 200.},
   {200.,  25., 200., 200.,  25.},
   { 25., 200.,  25.,  25., 200.},
   { 25., 200.,  25.,  25., 200.},
   {200.,  25., 200., 200.,  25.}
  };


/*const PS::F64 COEF_REP[5][5] =
  {{ 25., 150.,  25.,  25., 150.},
   {150.,  25., 150., 150.,  25.},
   { 25., 150.,  25.,  25., 150.},
   { 25., 150.,  25.,  25., 150.},
   {150.,  25., 150., 150.,  25.}
   };*/


/*const PS::F64 COEF_REP[5][5] =
  {{ 25., 100.,  25.,  25., 100.},
   {100.,  25., 100., 100.,  25.},
   { 25., 100.,  25.,  25., 100.},
   { 25., 100.,  25.,  25., 100.},
   {100.,  25., 100., 100.,  25.}
   };*/


/*const PS::F64 COEF_REP[5][5] =
  {{ 25., 80., 25., 25., 80.},
   { 80., 25., 80., 80., 25.},
   { 25., 80., 25., 25., 80.},
   { 25., 80., 25., 25., 80.},
   { 80., 25., 80., 80., 25.}
   };*/


/*const PS::F64 COEF_REP[5][5] =
  {{ 25., 60., 25., 25., 60.},
   { 60., 25., 60., 60., 25.},
   { 25., 60., 25., 25., 60.},
   { 25., 60., 25., 25., 60.},
   { 60., 25., 60., 60., 25.}
   };*/









/*const PS::F64 COEF_REP[4][4] =
  {{ 25., 200.,  25.,  30.},
   {200.,  25., 200., 200.},
   { 25., 200.,  25.,  30.},
   { 30., 200.,  30.,  25.}
   };*/

/*const PS::F64 COEF_REP[4][4] =
  {{ 25., 40.,  25.,  30.},
   { 40., 25.,  40.,  40.},
   { 25., 40.,  25.,  30.},
   { 30., 40.,  30.,  25.}
  };*/

/*const PS::F64 COEF_REP[4][4] =
  {{ 25., 60.,  25.,  30.},
   { 60., 25.,  60.,  60.},
   { 25., 60.,  25.,  30.},
   { 30., 60.,  30.,  25.}
   };*/

/*const PS::F64 COEF_REP[4][4] =
  {{ 25., 100.,  25.,  30.},
   { 100., 25.,  100.,  100.},
   { 25., 100.,  25.,  30.},
   { 30., 100.,  30.,  25.}
   };*/

/*const PS::F64 COEF_REP[4][4] =
  {{ 25., 150.,  25.,  30.},
   { 150., 25.,  150.,  150.},
   { 25., 150.,  25.,  30.},
   { 30., 150.,  30.,  25.}
   };*/

/*const PS::F64 COEF_REP[4][4] =
  {{ 25., 80.,  25.,  30.},
   { 80., 25.,  80.,  80.},
   { 25., 80.,  25.,  30.},
   { 30., 80.,  30.,  25.}
   };*/

/*const PS::F64 COEF_REP[4][4] =
  {{ 25., 70.,  25.,  30.},
   { 70., 25.,  70.,  70.},
   { 25., 70.,  25.,  30.},
   { 30., 70.,  30.,  25.}
   };*/

/*const PS::F64 COEF_REP[4][4] =
  {{ 25., 100.,  25.,  30.},
   { 100., 25.,  100.,  100.},
   { 25., 100.,  25.,  30.},
   { 30., 100.,  30.,  25.}
   };*/



const PS::F64 COEF_FRI = 4.5;
const PS::F64 COEF_BOND[2] = {128.,160.};
const PS::F64 BOND_LENGTH[2] = {0.7, 0.6};
const PS::F64 COEF_ANGL[2] = { 10.,  3.};

const PS::F64 RCUT = 1.0;
const PS::F64 RMERGIN = 2.0;
//const PS::F64 RMERGIN = 3.5;//add by kiyoshiro
//const PS::F64 RMERGIN = 0;//Reuse off
const PS::F64 RSEARCH = RCUT+RMERGIN;

#endif
