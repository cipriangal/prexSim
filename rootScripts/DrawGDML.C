{
  gSystem.Load("libGeom");
  TGeoManager::Import("prexII_std_coll.gdml");
  gGeoManager->GetTopVolume()->Draw("ogl");
}
