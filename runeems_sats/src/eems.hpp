#pragma once

#include "util.hpp"
#include "mcmc.hpp"
#include "draw.hpp"
#include "graph.hpp"
#include "habitat.hpp"

#include <boost/filesystem.hpp>


struct Proposal {
  int type;
  int qTile;
  int mTile;
  int newqtiles;
  int newmtiles;
  double newqEffct;
  double newmEffct;
  double newqSeedx;
  double newqSeedy;
  double newmSeedx;
  double newmSeedy;
  double newpi;
  double newll;
  double newmrateMu;
  VectorXd newtrDinvQxD;
  VectorXd newqEffcts;
  VectorXd newmEffcts;
  MatrixXd newqSeeds;
  MatrixXd newmSeeds;
  VectorXd newq;
  MatrixXd newB;
  VectorXi newqColors;
  VectorXi newmColors;
};

class EEMS {
public:

  EEMS(const Params &params, const long seed);
  ~EEMS( );

  void initialize(const MCMC &mcmc);
  double eval_prior( );
  double test_prior( ) const;
  double eval_likelihood( );
  double test_likelihood( ) const;
  void calc_q(const VectorXi &qColors0, const VectorXd &qEffcts0, VectorXd &q0) const;
  void calc_B(const VectorXi &mColors0, const VectorXd &mEffcts0, const double mrateMu0, MatrixXd &B0) const;
  double eval_proposal_qEffcts(Proposal &proposal) const;
  double eval_proposal_mEffcts(Proposal &proposal) const;
  double eval_proposal_mrateMu(Proposal &proposal) const;
  double eval_proposal_qSeeds(Proposal &proposal) const;
  double eval_proposal_mSeeds(Proposal &proposal) const;
  double eval_birthdeath_qVoronoi(Proposal &proposal) const;
  double eval_birthdeath_mVoronoi(Proposal &proposal) const;

  // Gibbs updates:
  void update_s2loc( );
  void update_hyperparams( );
  // Random-walk Metropolis-Hastings proposals:
  void propose_qEffcts(Proposal &proposal, const MCMC &mcmc);
  void propose_mEffcts(Proposal &proposal, const MCMC &mcmc);
  void propose_mrateMu(Proposal &proposal);
  void move_qVoronoi(Proposal &proposal, const MCMC &mcmc);
  void move_mVoronoi(Proposal &proposal, const MCMC &mcmc);
  void birthdeath_qVoronoi(Proposal &proposal, const MCMC &mcmc);
  void birthdeath_mVoronoi(Proposal &proposal, const MCMC &mcmc);
  bool accept_proposal(Proposal &proposal);
  ///////////////////////////////////////////

  bool save_iteration(const int iter);
  void report_iteration(const int iter) const;
  bool output_results(const MCMC &mcmc) const;
  void check_ll_computation() const;
  int num_qtiles( ) const;
  int num_mtiles( ) const;
  string datapath() const;
  string mcmcpath() const;
  double runif( );
  
private:

  Draw draw; // Random number generator
  Graph graph;
  Params params;
  Habitat habitat;

  int mtiles, qtiles;
  MatrixXd mSeeds; VectorXd mEffcts;
  MatrixXd qSeeds; VectorXd qEffcts;
  double qrateS2,mrateS2, mrateMu;
  double nowpi, nowll;

  // Diffs:
  int o; // observed demes
  int d; // all demes
  int n; // nIndiv
  int p; // nSites
  MatrixXd J;
  VectorXd s2loc;
  vector<MatrixXd> Diffs;
  vector<MatrixXd> L;
  vector<MatrixXd> Z;
  vector<MatrixXi> O;
  vector<VectorXi> ovec;
  // cvec, cinv, cmin1 are used to compute EEMS_wishpdfln
  vector<VectorXd> cvec; // c is the vector of counts
  vector<VectorXd> cinv;  // cinv is the vector of inverse counts
  vector<VectorXd> cmin1;  // cmin1 is the vector of counts - 1
  vector<MatrixXd> JtDobsJ;
  VectorXd c_allSites;
  MatrixXd JtDobsJ_allSites;
  MatrixXd JtDhatJ_allSites;
  double ldLLt; // logdet(L*L')
  double ldDiQ;  // logdet(inv(Diffs)*Q)
  double ldLDLt;  // logdet(-L*Diffs*L')
  VectorXd logn;
  VectorXd nmin1;
  void initialize_diffs();
  void runif_habitat(MatrixXd &Seeds);
  void rnorm_effects(const double HalfInterval, const double rateS2, VectorXd &Effcts);
  
  ///////////////////////////////////////////
  VectorXd nowtrDinvQxD;
  VectorXi nowqColors;
  VectorXi nowmColors;
  VectorXd nowq;
  MatrixXd nowB;
  double ll_partdf, qconst, Bconst;

  // Variables to store the results in :
  // Fixed size:
  int niters;
  MatrixXd mcmcmhyper;
  MatrixXd mcmcqhyper;
  MatrixXd mcmcthetas;
  MatrixXd mcmcpilogl;
  VectorXd mcmcmtiles;
  VectorXd mcmcqtiles;  
  // Variable length:
  vector<double> mcmcmRates;
  vector<double> mcmcqRates;
  vector<double> mcmcxCoord;
  vector<double> mcmcyCoord;
  vector<double> mcmcwCoord;
  vector<double> mcmczCoord;
  
  double EEMS_wishpdfln(const MatrixXd &B, const VectorXd &q, const VectorXd &s2loc, VectorXd &trDinvQxD) const;
  
};
