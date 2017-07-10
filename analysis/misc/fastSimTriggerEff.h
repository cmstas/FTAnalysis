double FastSimTriggerEfficiency(int pdgId1, int pdgId2, float pt1, float pt2, float eta1, float eta2, float ht) {
    return TotalTriggerEff(pdgId1, pt1, eta1, pdgId2, pt2, eta2, ht);
}

