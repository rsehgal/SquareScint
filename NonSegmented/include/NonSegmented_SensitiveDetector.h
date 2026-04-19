#ifndef NONSEGMENTED_SENSITIVEDETECTOR_HH
#define NONSEGMENTED_SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"

class G4HCofThisEvent;

class NonSegmented_SensitiveDetector : public G4VSensitiveDetector {
public:
    NonSegmented_SensitiveDetector(const G4String& name);
    virtual ~NonSegmented_SensitiveDetector();

    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);

    std::vector<std::vector<double>> vecOfArrivalTime_PMT;
    //std::vector<unsigned int> vecOfArrivalTime_PMT1;;
    //std::vector<unsigned int> vecOfArrivalTime_PMT2;
    //std::vector<unsigned int> vecOfArrivalTime_PMT3;
    //std::vector<unsigned int> vecOfArrivalTime_PMT4;

    void Initialize(G4HCofThisEvent *hitCollection);
    void EndOfEvent(G4HCofThisEvent *hitCollection);
};

#endif
