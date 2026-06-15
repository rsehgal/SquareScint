void enerRes()
{

  std::vector<float> energy = {340., 477., 1060.};
  std::vector<float> sigma  = {91.15, 163.65, 251.08};
  std::vector<float> sigmaEByE;
  for (short i = 0; i < 3; i++) {
    sigmaEByE.push_back(sigma[i] / energy[i]);
  }
for(short i=0;i<3;i++){
std::cout << sigmaEByE[i] << " ,";
}
std::cout << std::endl;
  TGraph *gr = new TGraph(energy.size(),&energy[0],&sigmaEByE[0]);
  gr->SetMarkerStyle(8);
  gr->Draw("ap");
}
