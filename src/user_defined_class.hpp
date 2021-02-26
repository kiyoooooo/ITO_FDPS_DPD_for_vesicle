#ifndef HPP_USER_DEFINED_CLASS
#define HPP_USER_DEFINED_CLASS

#include <particle_simulator.hpp>

#include <string>
#include <iostream>
#include <fstream>

#include "parameter.hpp"
#include "random_number.hpp"

struct Force{
  PS::F64vec force;
  PS::F64    pot;
  PS::F64    lipid_pot;//add by kiyoshiro
  PS::F32mat virial;
  void clear(){
    force = 0.0;
    pot = 0.0;
    lipid_pot = 0.0;
    virial = 0.f;
  }
};

struct FP{
  PS::F64vec pos;
  PS::F64vec vel;
  PS::F64vec vel2;
  PS::F64vec force;
  PS::F64 mass;
  PS::F64 pot;
  PS::F64 lipid_pot;

  PS::U32 id;
  PS::U32 type;

  PS::U32 nbond = 0;
  PS::U32 bpair[2];
  PS::U32 btype[2];

  PS::U32 nangle = 0;
  PS::U32 apair[2][3];
  PS::U32 atype[2];

  PS::F64vec getPos() const {return pos;}
  PS::F64vec getVel() const {return vel;}
  void setPos(const PS::F64vec _pos){pos = _pos;}
  void copyFromForce(const Force& f){
    force = f.force;
    pot = f.pot;
    lipid_pot = f.lipid_pot;
  }
};

struct EPI{
  PS::F32vec pos;
  PS::F32vec vel;
  PS::U32 type;

  PS::F32vec getPos() const {return pos; }
  PS::F32vec getVel() const {return vel; }
  void setPos(const PS::F64vec& _pos){ pos = _pos; }
  void copyFromFP(const FP& fp){
    pos  = fp.pos;
    vel  = fp.vel2;
    type = fp.type;
  }
};

struct EPJ{
  PS::F32vec pos;
  PS::F32vec vel;
  PS::U32 type;
  PS::U32 id;

  PS::F32vec getPos() const {return pos; }
  PS::F32vec getVel() const {return vel; }
  void setPos(const PS::F64vec& _pos){ pos = _pos; }
  void copyFromFP(const FP& fp){
    pos  = fp.pos;
    vel  = fp.vel2;
    id   = fp.id;
    type = fp.type;
  }
  PS::F32 getRSearch() const { return RSEARCH;}
  PS::S32 getId() const {return id;}
};

struct Config{
  PS::S32 nstep = 100000;
  PS::S32 ndstep1 = 100;
  PS::S32 ndstep2 = 100;
  PS::S32 nfnal = 1000;
  PS::S32 nstart = 0;
  PS::S32 nvstep = 0;
  PS::S32 nequil = 0;
  PS::S32 nwat = 15000;

  PS::F64 dt = 0.02;
  //  PS::F64 dt = 0.001;
  //PS::F64 temp = 1.15;
  PS::F64 temp = 1.0;//add by kiysohiro
  //  PS::F64 temp = 0.00001;
  //  PS::F64 temp_rand = 2.6;
  PS::F64 temp_rand = 1.0;//add by kiyoshiro

  PS::F32 z_ins = 36.0;

  Config(){if(ndstep1*nfnal > nstep) nfnal = nstep/ndstep1;}
  Config(const std::string filename){
    std::ifstream ifs(filename);
    ifs >> (*this);
    if(ndstep1*nfnal > nstep) nfnal = nstep/ndstep1;
  }

  void dump(std::ostream& os = std::cout) const {
#if 1
    os << " Number of Steps                                   : nstep    : i7    /" << std::setw(7) << nstep << "/" << std::endl;
    os << " Number of Steps between Output Lines for Position : ndstep1  : i7    /" << std::setw(7) << ndstep1 << "/" << std::endl;
    os << " Number of Steps between Output Lines for Pressure : ndstep2  : i7    /" << std::setw(7) << ndstep2 << "/" << std::endl;
    os << " Full Data Output Lines                            : nfnal    : i7    /" << std::setw(7) << nfnal << "/" << std::endl;
    os << " Starting Number of Step                           : nstart   : i7    /" << std::setw(7) << nstart << "/" << std::endl;
    os << " Number of Steps between Velocity Scalling         : nvstep   : i7    /" << std::setw(7) << nvstep << "/" << std::endl;
    os << " Number of Steps for Equil. lipid molecules        : nequil   : i7    /" << std::setw(7) << nequil << "/" << std::endl;
    os << " Number of water beads                             : nwat     : i7    /" << std::setw(7) << nwat << "/" << std::endl;
    os << std::endl;
    os << " Delta time                                        : delt     : f12.6 /" << std::setw(10) << dt << "d0/ ! 0.04" << std::endl;
    os << " Temperature                                       : set_temp : f12.6 /" << std::setw(10) << temp << "d0/ ! 1.0 ! 0.64" << std::endl;
    os << " Spred of Random Number                            : ran_temp : f12.6 /" << std::setw(10) << temp_rand << "d0/" << std::endl;

    os << " Insert position of polymer                        : z_ins    : f12.6 /" << std::setw(10) << z_ins << "d0/" << std::endl;
#else
    os << "nstep:\t" << nstep << std::endl;
    os << "ndstep1:\t" << ndstep1 << std::endl;
    os << "ndstep2:\t" << ndstep2 << std::endl;
    os << "nfnal:\t" << nfnal << std::endl;
    os << "nstart:\t" << nstart << std::endl;
    os << "nvstep:\t" << nvstep << std::endl;
    os << "nvstep:\t" << nvstep << std::endl;
    os << "nequil:\t" << nequil << std::endl;
    os << "nwat:\t" << nwat << std::endl;
    os << "dt:\t" << dt << std::endl;
    os << "temp:\t" << temp << std::endl;
    os << "temp_rand:\t" << temp_rand << std::endl;
    os << "z_ins:\t" << z_ins << std::endl;
#endif
  }

  friend std::istream& operator>>(std::istream& is, Config& rhs){
    std::string dummy;
    is >> dummy >> rhs.nstep;
    is >> dummy >> rhs.ndstep1;
    is >> dummy >> rhs.ndstep2;
    is >> dummy >> rhs.nfnal;
    is >> dummy >> rhs.nstart;
    is >> dummy >> rhs.nvstep;
    is >> dummy >> rhs.nequil;
    is >> dummy >> rhs.nwat;

    is >> dummy >> rhs.dt;
    is >> dummy >> rhs.temp;
    is >> dummy >> rhs.temp_rand;

    is >> dummy >> rhs.z_ins;

    return is;
  }
};


struct CalcForce{
  const PS::F64 dt;
  const PS::F64 temp;
  const PS::F64 temp_rand;
  CalcForce(const PS::F64 dt,const PS::F64 temp,const PS::F64 temp_rand):
    dt(dt),temp(temp),temp_rand(temp_rand){};
  void operator()(const EPI*    epi,
		  const PS::S32 nepi,
		  const EPJ*    epj,
		  const PS::S32 nepj,
		  Force*        force){
    static TEA rn;
    const PS::F32 rcut2 = RCUT*RCUT;
    const PS::F32 rcuti = 1.f / RCUT;
    const PS::F32 rdti = 1.f / sqrtf(dt);

    const PS::F32 sigma = sqrt(2.0*COEF_FRI*temp);
    const PS::F32 rtemp_rand = sqrt(temp_rand);
    const PS::F32 coef_rand = sigma * rtemp_rand * rdti;

    for(int i=0;i<nepi;i++){
      const PS::F32vec ri = epi[i].getPos();
      const PS::F32vec vi = epi[i].getVel();
      const PS::S32 ti = epi[i].type;
      PS::F64vec fi = 0.f;
      PS::F64 pot = 0.0;
      PS::F64 lipid_pot = 0.0;
      for(int j=0;j<nepj;j++){
	const PS::F32vec rj = epj[j].getPos();
	const PS::F32vec vj = epj[j].getVel();
	const PS::S32    tj = epj[j].type;
	const PS::F32vec dr = ri - rj;
	const PS::F32 r2 = dr*dr;
	if(r2 > rcut2 || r2 == 0.f) continue;

	const PS::F32 r = sqrt(r2);
	const PS::F32 ri  = 1.f / r;
	const PS::F32 r2i = ri * ri;
	PS::F32 f = 0.f;
	// conservation force
	const PS::F32 coef_rep = COEF_REP[ti][tj];
	const PS::F32 fc = coef_rep * (ri - rcuti);
	f += fc;
	pot += coef_rep*(r2*rcuti - 2.f*r + RCUT);
	if(ti==0 || ti ==1){lipid_pot += coef_rep*(r2*rcuti - 2.f*r + RCUT);}//add by kiyoshiro
	// dissipative force
	const PS::F32vec dv = vi - vj;
	const PS::F32 wij = 1.f - r*rcuti;
	const PS::F32 fv = COEF_FRI * wij * wij * (dr*dv) * r2i;
	f -= fv;
	// random force
	const PS::F32 fr = coef_rand * wij * ri * rn.drand((float)vi.x, (float)vj.x);
	f += fr;
	fi += dr * f;
      }
      force[i].force = fi;
      //      force[i].force = fi*0.0001;//made by kiyoshiro not to replace pos 
      //            force[i].force = fi*0.25;//made by kiyoshiro not to replace pos 
      //      force[i].force = 0;//made by kiyoshiro not to replace pos
      force[i].pot = 0.5*pot;
      force[i].lipid_pot = 0.5*lipid_pot;
    }
  }
};

Config CONFIG;

#endif
