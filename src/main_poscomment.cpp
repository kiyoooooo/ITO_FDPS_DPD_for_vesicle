#include<particle_simulator.hpp>

#include"user_defined_class.hpp"
#include"parameter.hpp"

#ifdef ENABLE_PEZY
#include"kernel.hpp"
#endif

#include<sys/stat.h>

#include<iostream>
#include<fstream>
#include<sstream>

bool isMasterProc(){
  return PS::Comm::getRank() == 0;
}

template<typename T>
void ReadCDVFile(T& psys,const std::string& filename){
  if(isMasterProc()){
    std::ifstream ifs(filename);
    assert(!ifs.fail());
    PS::S32 ncount = 0;
    for(std::string line;std::getline(ifs,line);){
      if(line[0] == '\'') continue;
      ncount++;
    }

    std::cerr << "# of particle is " << ncount << std::endl;
    psys.setNumberOfParticleLocal(ncount);
    ifs.clear();
    ifs.seekg(0,std::ios::beg);
    for(std::string line;std::getline(ifs,line);){
      if(line[0] == '\''){
	continue;
      }
      std::stringstream strs(line);
      PS::S32 i;
      strs >> i;
      i--;
      strs >> psys[i].type >> psys[i].pos.x >> psys[i].pos.y >> psys[i].pos.z;
      psys[i].type--;
      psys[i].id = i;
      psys[i].mass = 1.0;
    }
  }
}

template <typename Tpsys>
void WriteCDVFile(const Tpsys& psys, const std::string filename){
  PS::S32 myrank = PS::Comm::getRank();
  for(int p=0;p<PS::Comm::getNumberOfProc();p++){
    PS::Comm::barrier();
    if(myrank == p){
      if(p == 0){ std::ofstream ofs(filename);}
      std::ofstream ofs(filename,std::ios::app);
      if(p==0){
	ofs << "'box_sx=0. box_sy=0. box_sz=0."
	    << " box_ex=" << VOLUME.x
	    << " box_ey=" << VOLUME.y
	    << " box_ez=" << VOLUME.z
	    << " box_wt=.05" << std::endl;
	ofs << "'r1=0.4 r2=0.4 r3=0." << std::endl;
	ofs << "'c1=(0.5,0.5,1) c2=(1,0.5,0.5) c3=(0,1,1)" << std::endl;
	//ofs << "'bond_file=" << bond_file << std::endl;
	//ofs << "'bond_wt=0.1 bond_c=(0.8,0.8,0.8)" << std::endl;
      }
      for(int i=0;i<psys.getNumberOfParticleLocal();i++){
	//	ofs << std::setw(7) << i + 1;//is it OK? kiyo change this
	ofs << std::setw(7) << psys[i].id + 1;
	ofs << std::setw(2) << psys[i].type + 1;
	/*ofs << std::setw(19) << std::setprecision(15) << psys[i].pos.x;//we do not need 15 precision
	ofs << std::setw(19) << std::setprecision(15) << psys[i].pos.y;
	ofs << std::setw(19) << std::setprecision(15) << psys[i].pos.z << std::endl;*/
	ofs << std::setw(19) << std::setprecision(10) << psys[i].pos.x;
	ofs << std::setw(19) << std::setprecision(10) << psys[i].pos.y;
	ofs << std::setw(19) << std::setprecision(10) << psys[i].pos.z << std::endl;
      }
    }
  }
}

template<typename T>
void ReadVelFile(T& psys,const std::string& filename){
  if(isMasterProc()){
    std::ifstream ifs(filename);
    assert(!ifs.fail());
    PS::S32 ncount = 0;
    for(std::string line;std::getline(ifs,line);) ncount++;
    assert(ncount == psys.getNumberOfParticleLocal());

    ifs.clear();
    ifs.seekg(0,std::ios::beg);
    for(std::string line;std::getline(ifs,line);){
      std::stringstream strs(line);
      PS::S32 i;
      strs >> i;
      i--;
      strs >> psys[i].vel.x >> psys[i].vel.y >> psys[i].vel.z;
      psys[i].vel2 = psys[i].vel;
    }
  }
}

template <typename Tpsys>
void WriteVelFile(const Tpsys& psys, const std::string filename){
  PS::S32 myrank = PS::Comm::getRank();
  for(int p=0;p<PS::Comm::getNumberOfProc();p++){
    PS::Comm::barrier();
    if(myrank == p){
      if(p == 0){ std::ofstream ofs(filename);}
      std::ofstream ofs(filename,std::ios::app);
      for(int i=0;i<psys.getNumberOfParticleLocal();i++){
	ofs << std::setw(7) << psys[i].id + 1
	    << " " << std::setw(19) << std::setprecision(15) << psys[i].vel.x
	    << " " << std::setw(19) << std::setprecision(15) << psys[i].vel.y
	    << " " << std::setw(19) << std::setprecision(15) << psys[i].vel.z << std::endl;
      }
    }
  }
}

template<typename T>
void ReadBondFile(T& psys,const std::string& filename){
  if(isMasterProc()){
    std::ifstream ifs(filename);
    assert(!ifs.fail());
    for(std::string line;std::getline(ifs,line);){
      PS::S32 i,j;
      std::stringstream strs(line);
      strs >> i >> j;
      i--; j--;
      assert(psys[i].nbond < 2);
      assert(psys[j].nbond < 2);

      psys[i].bpair[psys[i].nbond] = j;
      psys[i].btype[psys[i].nbond] = 0;
      psys[i].nbond++;


      psys[j].bpair[psys[j].nbond] = i;
      psys[j].btype[psys[j].nbond] = 0;
      psys[j].nbond++;

    }
  }
}

template<typename T>
void ReadAngleFile(T& psys,const std::string& filename){
  if(isMasterProc()){
    std::ifstream ifs(filename);
    assert(!ifs.fail());
    for(std::string line;std::getline(ifs,line);){
      PS::S32 i,j,k,type;
      std::stringstream strs(line);
      strs >> i >> j >> k >> type;
      i--; j--; k--; type--;
      assert(psys[i].nangle < 2);
      assert(psys[j].nangle < 2);
      assert(psys[k].nangle < 2);

      psys[i].apair[psys[i].nangle][0] = i;
      psys[i].apair[psys[i].nangle][1] = j;
      psys[i].apair[psys[i].nangle][2] = k;
      psys[i].atype[psys[i].nangle] = type;
      psys[i].nangle++;


      psys[j].apair[psys[j].nangle][0] = i;
      psys[j].apair[psys[j].nangle][1] = j;
      psys[j].apair[psys[j].nangle][2] = k;
      psys[j].atype[psys[j].nangle] = type;
      psys[j].nangle++;

      psys[k].apair[psys[k].nangle][0] = i;
      psys[k].apair[psys[k].nangle][1] = j;
      psys[k].apair[psys[k].nangle][2] = k;
      psys[k].atype[psys[k].nangle] = type;
      psys[k].nangle++;

    }
  }
}

template <typename Tpsys>
void WriteForceFile(const Tpsys& psys,const std::string filename){
  PS::S32 myrank = PS::Comm::getRank();
  for(int p=0;p<PS::Comm::getNumberOfProc();p++){
    PS::Comm::barrier();
    if(p == myrank){
      if(p == 0){ std::ofstream ofs(filename);}
      std::ofstream ofs(filename,std::ios::app);
      assert(!ofs.fail());
      for(int i=0;i<psys.getNumberOfParticleLocal();i++){
	const PS::F64vec f = psys[i].force;
	const PS::F64    p = psys[i].pot;
	ofs << std::setw(7) << psys[i].id + 1
	    << " " << std::setw(21) << std::setprecision(15) << f
	    << " " << std::setw(21) << std::setprecision(15) << p << std::endl;
      }
    }
  }
}

template <typename Tpsys>
void WritePotential(const Tpsys& psys,const std::string filename){
  PS::S32 myrank = PS::Comm::getRank();
  for(int p=0;p<PS::Comm::getNumberOfProc();p++){
    PS::Comm::barrier();
    if(p == myrank){
      if(p == 0){ std::ofstream ofs(filename);}
      std::ofstream ofs(filename,std::ios::app);
      assert(!ofs.fail());
      for(int i=0;i<psys.getNumberOfParticleLocal();i++){
	const PS::F64 p = psys[i].pot;
	const PS::F64 b = psys[i].bond;
	const PS::F64 a = psys[i].angle;
	ofs << std::setw(7) << psys[i].id + 1
	    << " " << std::setw(21) << std::setprecision(15) << p
	    << " " << std::setw(21) << std::setprecision(15) << b
	    << " " << std::setw(21) << std::setprecision(15) << a << std::endl;
      }
    }
  }
}

template <typename Tpsys>
void InsertPolymer(Tpsys& psys,const Config& cfg){
  std::random_device seed_gen;
  std::mt19937_64 engine(seed_gen());
  std::uniform_real_distribution<> rand(-0.5,0.5);

  assert(psys.getNumberOfParticleLocal() > MOL_T);

  psys[MOL_T].pos.x = 0.5 * VOLUME.x;
  psys[MOL_T].pos.y = 0.5 * VOLUME.y;
  psys[MOL_T].pos.z = cfg.z_ins;
  psys[MOL_T].type = 3;

  for(int i=MOL_T+1;i<MOL_T+NCHAIN_POLYMER;i++){
    PS::F64 r2 = 100.;
    PS::F64vec tmp = psys[i].pos;
    while(r2 > 3.5*3.5){
      const PS::F64vec dr = PS::F64vec(rand(engine),rand(engine),rand(engine));
      const PS::F64 r = sqrt(dr*dr);
      tmp = psys[i-1].pos + dr*(0.5/r);
      const PS::F64vec cr = tmp - psys[MOL_T].pos;
      r2 = cr*cr;
    }
    psys[i].pos  = tmp;
    psys[i].type = 3;
  }
}

template <typename Tpsys>
void PutWaterInside(Tpsys& psys, const Config& cfg){
  std::random_device seed_gen;
  std::mt19937_64 engine(seed_gen());
  std::uniform_real_distribution<> rand(0.0,1.0);

  PS::S32 wcount = 0;
  for(int i=MOL_T+NCHAIN_POLYMER;i<psys.getNumberOfParticleLocal();i++){
    PS::F64vec2 dr;
    dr.x = psys[i].pos.x - 0.5*VOLUME.x;
    dr.y = psys[i].pos.y - 0.5*VOLUME.y;
    PS::F64 r2 = dr*dr;
    if(cfg.nwat > 0){
      if(r2 > OUTER_RADIUS2){
	while(r2 < INNER_RADIUS2){
	  psys[i].pos.x = rand(engine)*VOLUME.x;
	  psys[i].pos.y = rand(engine)*VOLUME.y;
	  dr.x = psys[i].pos.x - 0.5*VOLUME.x;
	  dr.y = psys[i].pos.y - 0.5*VOLUME.y;
	  r2 = dr*dr;
	}
	psys[i].pos.z = rand(engine)*VOLUME.z;
	wcount++;
      }
    }else if(cfg.nwat < 0){
      if(r2 < INNER_RADIUS2){
	while(r2 > OUTER_RADIUS2){
	  psys[i].pos.x = rand(engine)*VOLUME.x;
	  psys[i].pos.y = rand(engine)*VOLUME.y;
	  dr.x = psys[i].pos.x - 0.5*VOLUME.x;
	  dr.y = psys[i].pos.y - 0.5*VOLUME.y;
	  r2 = dr*dr;
	}
	psys[i].pos.z = rand(engine)*VOLUME.z;
	wcount++;
      }
    }
    if(wcount == cfg.nwat) break;
  }
}

template <typename Tpsys,typename Ttree>
void CalcInternalForce(Tpsys& psys,Ttree& tree,PS::F64& ebond, PS::F64& eangle){
  ebond = eangle = 0.0;
  for(int i=0;i<psys.getNumberOfParticleLocal();i++){
    const PS::F64vec ri = psys[i].getPos();
    PS::F64vec fi = 0.0;
    for(int j=0;j<psys[i].nbond;j++){
      const PS::F64 coef_bond = COEF_BOND[psys[i].btype[j]];
      const PS::F64 len_bond = BOND_LENGTH[psys[i].btype[j]];
      const EPJ* epj = tree.getEpjFromId(psys[i].bpair[j]);
      assert(epj != NULL);
      PS::F64vec dr = ri - epj->getPos();
      while(dr.x < -0.5*VOLUME.x) dr.x += VOLUME.x;
      while(dr.y < -0.5*VOLUME.y) dr.y += VOLUME.y;
      while(dr.z < -0.5*VOLUME.z) dr.z += VOLUME.z;
      while(dr.x >= 0.5*VOLUME.x) dr.x -= VOLUME.x;
      while(dr.y >= 0.5*VOLUME.y) dr.y -= VOLUME.y;
      while(dr.z >= 0.5*VOLUME.z) dr.z -= VOLUME.z;
      assert(-0.5*VOLUME.x <= dr.x && dr.x < 0.5*VOLUME.x);
      assert(-0.5*VOLUME.y <= dr.y && dr.y < 0.5*VOLUME.y);
      assert(-0.5*VOLUME.z <= dr.z && dr.z < 0.5*VOLUME.z);

      const PS::F64 r = sqrt(dr*dr);
      const PS::F64 dx = len_bond - r;
      fi += dr * (coef_bond*dx);
      ebond += coef_bond*dx*dx;
    }
    for(int j=0;j<psys[i].nangle;j++){
      const PS::S32 myid = psys[i].id;
      const PS::S32 i0 = psys[i].apair[j][0];
      const PS::S32 i1 = psys[i].apair[j][1];
      const PS::S32 i2 = psys[i].apair[j][2];
      const EPJ* epj0 = tree.getEpjFromId(i0);
      const EPJ* epj1 = tree.getEpjFromId(i1);
      const EPJ* epj2 = tree.getEpjFromId(i2);
      assert(epj0 != NULL);
      assert(epj1 != NULL);
      assert(epj2 != NULL);

      const PS::F64vec r0 = epj0->getPos();
      const PS::F64vec r1 = epj1->getPos();
      const PS::F64vec r2 = epj2->getPos();

      PS::F64vec dr01 = r0 - r1;
      PS::F64vec dr21 = r2 - r1;
      while(dr01.x < -0.5*VOLUME.x) dr01.x += VOLUME.x;
      while(dr01.y < -0.5*VOLUME.y) dr01.y += VOLUME.y;
      while(dr01.z < -0.5*VOLUME.z) dr01.z += VOLUME.z;
      while(dr01.x >= 0.5*VOLUME.x) dr01.x -= VOLUME.x;
      while(dr01.y >= 0.5*VOLUME.y) dr01.y -= VOLUME.y;
      while(dr01.z >= 0.5*VOLUME.z) dr01.z -= VOLUME.z;
      assert(-0.5*VOLUME.x <= dr01.x && dr01.x < 0.5*VOLUME.x);
      assert(-0.5*VOLUME.y <= dr01.y && dr01.y < 0.5*VOLUME.y);
      assert(-0.5*VOLUME.z <= dr01.z && dr01.z < 0.5*VOLUME.z);
      while(dr21.x < -0.5*VOLUME.x) dr21.x += VOLUME.x;
      while(dr21.y < -0.5*VOLUME.y) dr21.y += VOLUME.y;
      while(dr21.z < -0.5*VOLUME.z) dr21.z += VOLUME.z;
      while(dr21.x >= 0.5*VOLUME.x) dr21.x -= VOLUME.x;
      while(dr21.y >= 0.5*VOLUME.y) dr21.y -= VOLUME.y;
      while(dr21.z >= 0.5*VOLUME.z) dr21.z -= VOLUME.z;
      assert(-0.5*VOLUME.x <= dr21.x && dr21.x < 0.5*VOLUME.x);
      assert(-0.5*VOLUME.y <= dr21.y && dr21.y < 0.5*VOLUME.y);
      assert(-0.5*VOLUME.z <= dr21.z && dr21.z < 0.5*VOLUME.z);

      const PS::F64 rsq01 = dr01*dr01;
      const PS::F64 rsq21 = dr21*dr21;
      const PS::F64 r01 = sqrt(rsq01);
      const PS::F64 r21 = sqrt(rsq21);
      const PS::F64 cost = std::max(std::min(dr01 * dr21 / (r01*r21),1.0),-1.0);
      const PS::F64 sint = std::max(sqrt(1.0 - cost*cost),0.001);
      const PS::F64 dtheta = acos(cost) - M_PI;
      const PS::F64 tk = COEF_ANGL[psys[i].atype[j]]*dtheta;
      const PS::F64 aa = 2.0*tk*sint;

      if(myid == i0) fi += dr21*(aa/(r01*r21)) - dr01*(aa*cost/rsq01);
      if(myid == i1) fi -= (dr01+dr21)*(aa/(r01*r21)) - dr01*(aa*cost/rsq01) - dr21*(aa*cost/rsq21);
      if(myid == i2) fi += dr01*(aa/(r01*r21)) - dr21*(aa*cost/rsq21);
      eangle += tk*dtheta;
    }
    psys[i].force += fi;
  }
  ebond  *= 0.25;
  eangle /= 3.0;
}

template <typename Tpsys>
void ModifiedVelVerlet0(Tpsys& psys,const Config& cfg){
  for(int i=0;i<psys.getNumberOfParticleLocal();i++){
    PS::F64 massi = 1.0 / psys[i].mass;
    psys[i].vel2 = psys[i].vel + psys[i].force * (0.65*massi*cfg.dt);
    psys[i].vel += psys[i].force * (0.5*massi*cfg.dt);
    psys[i].pos += psys[i].vel * cfg.dt;
  }
}

template <typename Tpsys>
void ModifiedVelVerlet1(Tpsys& psys,const Config& cfg){
  for(int i=0;i<psys.getNumberOfParticleLocal();i++){
    psys[i].vel += psys[i].force * (0.5/psys[i].mass*cfg.dt);
  }
}

template <typename Tpsys>
void CalcRadiusOfTube(const Tpsys& psys,PS::F64& raius,PS::F64& density){
  
}

template <typename Tpsys>
PS::F64 ReducePotentialEnergy(const Tpsys& psys){
  PS::F64 pot = 0.0;
  for(int i=0;i<psys.getNumberOfParticleLocal();i++) pot += psys[i].pot;
  return 0.5*PS::Comm::getSum(pot);
}

template <typename Tpsys>
PS::F64 CalcKineticEnergy(const Tpsys& psys){
  PS::F64 kin = 0.0;
  for(int i=0;i<psys.getNumberOfParticleLocal();i++){
    kin += psys[i].mass*(psys[i].vel*psys[i].vel);
  }
  return 0.5 * PS::Comm::getSum(kin);
}

void makeOutputDirectory(std::string dir_name) {
  struct stat st;
  PS::S32 ret = 0;
  if (isMasterProc()) {
    if (stat(dir_name.c_str(), &st) != 0) {
      ret = mkdir(dir_name.c_str(), 0777);
    } else {
      ret = 0; // the directory named dir_name already exists.
    }
  }
  PS::Comm::broadcast(&ret, 1);
  if (ret == 0) {
    if (isMasterProc())
      std::cerr << "Directory " << dir_name << " is successfully made." << std::endl;
  } else {
    if (isMasterProc())
      std::cerr << "Directory " << dir_name << " fails to be made.";
    PS::Abort();
  }
}

int main(int argc, char **argv){
  //  if(isMasterProc()) std::cout << "OK1" << std::endl;//add by kiyoshiro to know where is the error
  PS::Initialize(argc,argv);

  // FDPS parameters
  const PS::F64 theta = 0.0; // theta must be 0.0
  PS::S32 n_group_limit = 8;
  PS::S32 n_leaf_limit  = 1;
  PS::S32 n_walk_limit  = 256;
  // input files
  std::string output_dir  = "./result/";
  std::string input_dir   = "./orig/";
  std::string cdv_input   = "dpd_w_00000.dat";
  std::string vel_input   = "dpd_00000_vel.dat";
  std::string bond_input  = "dpd_bond.dat.org";
  std::string angle_input = "dpd_angle.dat";
  // simulation setting parameter

  for(int i=1; i < argc; i++){
    std::string opt = argv[i];
    // input output options
    if(opt == "-x"){
      cdv_input = argv[++i];
      if(isMasterProc()) std::cout << "cdv_input:\t" << cdv_input << std::endl;
      continue;
    }
    if(opt == "-v"){
      vel_input = argv[++i];
      if(isMasterProc())std::cout << "vel_input:\t" << vel_input << std::endl;
      continue;
    }
    if(opt == "-b"){
      bond_input = argv[++i];
      if(isMasterProc())std::cout << "bond_input:\t" << bond_input << std::endl;
      continue;
    }
    if(opt == "-a"){
      angle_input = argv[++i];
      if(isMasterProc())std::cout << "angle_input:\t" << angle_input << std::endl;
      continue;
    }
    if(opt == "-i"){
      input_dir = argv[++i];
      input_dir += "/";
      if(isMasterProc()) std::cout << "input_dir:\t" << input_dir << std::endl;
      continue;
    }
    if(opt == "-o"){
      output_dir = argv[++i];
      output_dir += "/";
      if(isMasterProc())std::cout << "output_dir:\t" << output_dir << std::endl;
      continue;
    }

    // simulation parameter options
    if(opt == "-s"){
      CONFIG.nstep = atoi(argv[++i]);
      if(isMasterProc())std::cerr << "nstep:\t" << CONFIG.nstep << std::endl;
      continue;
    }
    if(opt == "-d"){
      CONFIG.ndstep2 = atoi(argv[++i]);
      if(isMasterProc())std::cerr << "ndstep2:\t" << CONFIG.ndstep2 << std::endl;
      continue;
    }
    if(opt == "-r"){
      CONFIG.temp_rand = atof(argv[++i]);
      if(isMasterProc())std::cerr << "temp_rand:\t" << CONFIG.temp_rand << std::endl;
      continue;
    }

    // FDPS parameter options
    if(opt == "-g"){
      n_group_limit = atoi(argv[++i]);
      if(isMasterProc())std::cout << "n_group_limit:\t" << n_group_limit << std::endl;
      continue;
    }
    if(opt == "-l"){
      n_leaf_limit = atoi(argv[++i]);
      if(isMasterProc())std::cout << "n_leaf_limit:\t" << n_leaf_limit << std::endl;
      continue;
    }
    if(opt == "-w"){
      n_walk_limit = atoi(argv[++i]);
      if(isMasterProc())std::cout << "n_walk_limit:\t" << n_walk_limit << std::endl;
      continue;
    }
    if(opt == "-h" || opt == "--help"){
      std::cerr << "usage: " << argv[0]
		<< " -x [cdview file]"
		<< " -v [velocity file]"
		<< " -b [bond file]"
		<< " -a [angle file]"
		<< " -o [output directory]"
		<< " -s [nuber of total step]"
		<< " -d [interval of output]"
		<< " -r [temperature for random force]"
		<< " -g [n_group_limit]"
		<< " -l [n_leaf_limit]"
		<< " -w [n_walk_limit]" << std::endl;
      PS::Abort();
    }
    std::cerr << "error: undefined option " << opt << std::endl;
    PS::Abort();
  }

  //  if(isMasterProc()) std::cout << "OK2" << std::endl;//add by kiyoshiro to know where is the error
  
  makeOutputDirectory(output_dir);
  //  if(isMasterProc()) cfg.dump(std::cerr);//コンパイルエラーのため，一旦コメント化2020/05/04kiyoshirookada
  
  PS::ParticleSystem<FP> system_dpd;
  PS::DomainInfo dinfo;
  PS::TreeForForceShort<Force,EPI,EPJ>::Scatter tree_dpd;
  
  
  //  if(isMasterProc()) std::cout << "OK3" << std::endl;//add by kiyoshiro to know where is the error
  
  system_dpd.initialize();
  //  if(isMasterProc()) std::cout << "OK4" << std::endl;//add by kiyoshiro to know where is the error
  ReadCDVFile(system_dpd,input_dir+cdv_input);
  //InsertPolymer(system_dpd,CONFIG);
  //PutWaterInside(system_dpd,CONFIG);
  ReadVelFile(system_dpd,input_dir+vel_input);
  ReadBondFile(system_dpd,input_dir+bond_input);
  ReadAngleFile(system_dpd,input_dir+angle_input);
  
  //  if(isMasterProc()) std::cout << "OK5" << std::endl;//add by kiyoshiro to know where is the error
  
  dinfo.initialize(0.3);
  
  //  if(isMasterProc()) std::cout << "OK6" << std::endl;//add by kiyoshiro to know where is the error
  
  
  dinfo.setBoundaryCondition(PS::BOUNDARY_CONDITION_PERIODIC_XYZ);
  
  //  if(isMasterProc()) std::cout << "OK7" << std::endl;//add by kiyoshiro to know where is the error
  dinfo.setPosRootDomain(PS::F64vec(0,0,0),VOLUME);
  //  if(isMasterProc()) std::cout << "OK8" << std::endl;//add by kiyoshiro to know where is the error
  
  system_dpd.adjustPositionIntoRootDomain(dinfo);
  //  if(isMasterProc()) std::cout << "OK9" << std::endl;//add by kiyoshiro to know where is the error
  
  dinfo.decomposeDomainAll(system_dpd);
  //  if(isMasterProc()) std::cout << "OK10" << std::endl;//add by kiyoshiro to know where is the error
  system_dpd.exchangeParticle(dinfo);
  
  //  if(isMasterProc()) std::cout << "OK11" << std::endl;//add by kiyoshiro to know where is the error
  tree_dpd.initialize(NMOL*NATOM,theta,n_leaf_limit,n_group_limit);
  //  if(isMasterProc()) std::cout << "OK12" << std::endl;//add by kiyoshiro to know where is the error
  tree_dpd.calcForceAllAndWriteBack(CalcForce(CONFIG.dt,CONFIG.temp,CONFIG.temp_rand),system_dpd,dinfo);
  //  if(isMasterProc()) std::cout << "OK13" << std::endl;//add by kiyoshiro to know where is the error
  PS::F64 ebond,eangle;
  CalcInternalForce(system_dpd,tree_dpd,ebond,eangle);
  //  if(isMasterProc()) std::cout << "OK14" << std::endl;//add by kiyoshiro to know where is the error
  
  //PS::F64* tube_radius = new PS::F64[60];
  //PS::F64* density = new PS::F64[];
  std::ofstream ofs_eng;
  PS::INTERACTION_LIST_MODE reuse_mode = PS::MAKE_LIST;
#ifdef PARTICLE_SIMULATOR_REUSE_LIST
  const PS::S32 reuse_interval = 10;
  if(isMasterProc())
    std::cerr << "REUSE_MODE is enabled (interval is " << reuse_interval << ")." << std::endl;
#else
  const PS::S32 reuse_interval = 1;
#endif
  if(isMasterProc()) ofs_eng.open(output_dir+"eng.dat");
  system_dpd.clearTimeProfile();
  dinfo.clearTimeProfile();
  tree_dpd.clearTimeProfile();
  
  // main loop
#ifdef ENABLE_PEZY
  const PS::S32 tag = 1;
  std::cerr << "PEZY-SC2 is ENABLED" << std::endl;
  InitializeDEVICE();
#endif
  if(isMasterProc()) std::cout << "OK15" << std::endl;//add by kiyoshiro to know where is the error
  PS::Comm::barrier();//add by kiyoshiro adviced by nomura-san
  const PS::F64 time_offset = PS::GetWtime();
  PS::F64 time_calc_internal_force = 0.0;//add by kiyoshiro adviced by nomura-san
  for(int s=0; s<CONFIG.nstep; s++){
    PS::Comm::barrier();//add by kiyoshiro adviced by nomura-san
#ifdef PARTICLE_SIMULATOR_REUSE_LIST
    if(s%reuse_interval == 0) reuse_mode = PS::MAKE_LIST_FOR_REUSE;
    else                      reuse_mode = PS::REUSE_LIST;
#endif
    ModifiedVelVerlet0(system_dpd,CONFIG);
    if(isMasterProc()) std::cout << "OK16 and s = " << s << std::endl;//add by kiyoshiro to know where is the error
    
    if(s%reuse_interval==0){
      system_dpd.adjustPositionIntoRootDomain(dinfo);
      dinfo.decomposeDomainAll(system_dpd);
      system_dpd.exchangeParticle(dinfo);
    }
    
    if(isMasterProc()) std::cout << "OK17 and s = " << s << std::endl;//add by kiyoshiro to know where is the error


    /*#ifdef ENABLE_PEZY
    tree_dpd.calcForceAllAndWriteBackMultiWalkIndex
      (DispatchKernelIndex, RetrieveKernel,
       tag,system_dpd,dinfo,n_walk_limit,true,reuse_mode);
    
    if(s%reuse_interval==0){
      system_dpd.adjustPositionIntoRootDomain(dinfo);
      dinfo.decomposeDomainAll(system_dpd);
      system_dpd.exchangeParticle(dinfo);
      }*///なぜ追加されたかわからない
    
#ifdef ENABLE_PEZY
    tree_dpd.calcForceAllAndWriteBackMultiWalkIndex
      (DispatchKernelIndex, RetrieveKernel,
       tag,system_dpd,dinfo,n_walk_limit,true,reuse_mode);
#else
    PS::Comm::barrier();//add by kiyoshiro adviced by nomura-san
    tree_dpd.calcForceAllAndWriteBack
      (CalcForce(CONFIG.dt,CONFIG.temp,CONFIG.temp_rand),
       system_dpd,dinfo,true,reuse_mode);
#endif
    if(isMasterProc()) std::cout << "OK18 and s = " << s << std::endl;//add by kiyoshiro to know where is the error
    CalcInternalForce(system_dpd,tree_dpd,ebond,eangle);
    
    if(isMasterProc()) std::cout << "OK19 and s = " << s << std::endl;//add by kiyoshiro to know where is the error
    PS::F64 offset_calc_internal_force = PS::GetWtime();//add by kiyoshiro adviced by nomura-san
    CalcInternalForce(system_dpd,tree_dpd,ebond,eangle);//add by kiyoshiro adviced by nomura-san
    time_calc_internal_force += PS::GetWtime() - offset_calc_internal_force;//add by kiyoshiro adviced by nomura-san
    ModifiedVelVerlet1(system_dpd,CONFIG);
    if(isMasterProc()) std::cout << "OK20 and s = " << s << std::endl;//add by kiyoshiro to know where is the error
    /*    if(s%CONFIG.ndstep2==0){
	  const PS::F64 epot = ReducePotentialEnergy(system_dpd);
	  const PS::F64 ekin = CalcKineticEnergy(system_dpd);
	  ebond  = PS::Comm::getSum(ebond);
	  eangle = PS::Comm::getSum(eangle);
	  if(isMasterProc()){
	  if(s==0) ofs_eng << "# step pot bond angle (pot+bond+angle) kin" << std::endl;
	  ofs_eng << std::setw(7) <<  s
	  << std::setw(22) << std::setprecision(15) << epot/NMOL
	  << std::setw(22) << std::setprecision(15) << ebond/NMOL
	  << std::setw(22) << std::setprecision(15) << eangle/NMOL
	  << std::setw(22) << std::setprecision(15) << (epot+ebond+eangle)/NMOL
	  << std::setw(22) << std::setprecision(15) << ekin/NMOL/1.5 << std::endl;
	  }
	  
	  std::stringstream cdv,vel;
	  cdv << output_dir << std::setw(7) << std::setfill('0') << s/CONFIG.ndstep2 << ".cdv";
	  WriteCDVFile(system_dpd,cdv.str());
	  vel << output_dir << std::setw(7) << std::setfill('0') << s/CONFIG.ndstep2 << ".vel";
	  WriteVelFile(system_dpd,vel.str());
	  #if 0
	  std::stringstream force;
	  force << output_dir << std::setw(7) << std::setfill('0') << s/CONFIG.ndstep2 << ".force";
	  WriteForceFile(system_dpd,force.str());
	  #endif
	  //CalcRadiusOfTube(system_dpd,tube_radius,density);
	  }*/  //commented out by kiyoshiro adviced by nomura-san
  }
  PS::Comm::barrier();
  const PS::F64 total_time = PS::GetWtime() - time_offset;
  const PS::TimeProfile tp = system_dpd.getTimeProfile() + dinfo.getTimeProfile() + tree_dpd.getTimeProfile();
  std::stringstream ss_tp;
  ss_tp << output_dir << "time_profile" << std::setw(3) << std::setfill('0') << PS::Comm::getRank() << ".dat";
  std::ofstream ofs_tp(ss_tp.str());
  tp.dump(ofs_tp);
  ofs_tp << "CalcInternalForce=" << time_calc_internal_force << std::endl;//add by kiyoshiro adviced by nomura-san
  ofs_tp << "TotalElapsedTime: " << total_time << std::endl;
  
  PS::Finalize();
  return 0;
}
