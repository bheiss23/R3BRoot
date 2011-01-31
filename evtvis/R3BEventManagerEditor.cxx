#include "R3BEventManagerEditor.h"

#include "R3BEventManager.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "TGLabel.h"
#include "TGButton.h"
#include "TGWindow.h"
#include "TGNumberEntry.h"
#include "TGeoManager.h"
#include "TChain.h"
#include "TEveGValuators.h"

ClassImp(R3BEventManagerEditor)


R3BEventManagerEditor::R3BEventManagerEditor(const TGWindow* p, Int_t width, Int_t height,
                      UInt_t options, Pixel_t back)
: FairEventManagerEditor(p,width,height,options,back){}


void R3BEventManagerEditor::Init(){

    FairRootManager *fRootManager=FairRootManager::Instance();
   TChain *chain =fRootManager->GetInChain();
   Int_t Entries= chain->GetEntriesFast();

   MakeTitle("R3BEventManager  Editor");
   TGVerticalFrame      *fInfoFrame= CreateEditorTabSubFrame("Info");
   TGCompositeFrame *title1 = new TGCompositeFrame(fInfoFrame, 250, 10,
                                                   kVerticalFrame | kLHintsExpandX |
                                                   kFixedWidth    | kOwnBackground);

   TString Infile= "Input file : ";
   TFile *file =FairRunAna::Instance()->GetInputFile();
   Infile+=file->GetName();
   TGLabel *TFName=new TGLabel(title1, Infile.Data());
   title1->AddFrame(TFName);

   UInt_t RunId= FairRunAna::Instance()->getRunId();
   TString run= "Run Id : ";
   run += RunId;
   TGLabel *TRunId=new TGLabel(title1, run.Data());
   title1->AddFrame( TRunId);

   TString nevent= "No of events : ";
   nevent +=Entries ;
   TGLabel *TEvent=new TGLabel(title1, nevent.Data());
   title1->AddFrame(TEvent);

   Int_t nodes= gGeoManager->GetNNodes();
   TString NNodes= "No. of Nodes : ";
   NNodes += nodes;
   TGLabel *NoNode=new TGLabel(title1, NNodes.Data());
   title1->AddFrame( NoNode);

   TGHorizontalFrame* f = new TGHorizontalFrame(title1);
   TGLabel *l = new TGLabel(f, "Current Event:");
   f->AddFrame(l, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 1, 2, 1, 1));
   fCurrentEvent = new TGNumberEntry(f, 0., 6, -1,
                                        TGNumberFormat::kNESInteger, 
				        TGNumberFormat::kNEANonNegative,
                                        TGNumberFormat::kNELLimitMinMax, 0, Entries);
   f->AddFrame(fCurrentEvent, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
   fCurrentEvent->Connect("ValueSet(Long_t)","R3BEventManagerEditor", this, "SelectEvent()");
   title1->AddFrame(f);

   fVizPri = new TGCheckButton(title1, "Primary Only");
   AddFrame(fVizPri, new TGLayoutHints(kLHintsTop, 3, 1, 1, 0));
   fVizPri->Connect("Toggled(Bool_t)", "R3BEventManagerEditor", this, "DoVizPri()");

   title1->AddFrame(fVizPri);

   TGHorizontalFrame* f1 = new TGHorizontalFrame(title1);
   TGLabel *L1 = new TGLabel(f1, "Select PDG :");
   f1->AddFrame(L1, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 1, 2, 1, 1));
   fCurrentPDG = new TGNumberEntry(f1, 0., 12, -1,
                                        TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber,
                                        TGNumberFormat::kNELNoLimits, 0, 1);
   f1->AddFrame(fCurrentPDG, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
   fCurrentPDG->Connect("ValueSet(Long_t)","R3BEventManagerEditor", this, "SelectPDG()");
   title1->AddFrame(f1);


   fMinEnergy = new TEveGValuator(title1, "Min Energy:", 90, 0);
   fMinEnergy->SetNELength(7);
   fMinEnergy->SetLabelWidth(80);
   fMinEnergy->Build();
   fMinEnergy->SetLimits(0, 5000, 500001, TGNumberFormat::kNESRealOne);
   fMinEnergy->SetToolTip("Minimum energy of displayed track. [MeV]");
   fMinEnergy->Connect("ValueSet(Double_t)", "R3BEventManagerEditor",this, "MinEnergy()");
   title1->AddFrame(fMinEnergy, new TGLayoutHints(kLHintsTop, 1, 1, 1, 0));


   fMaxEnergy = new TEveGValuator(title1, "Max Energy:", 90, 0);
   fMaxEnergy->SetNELength(7);
   fMaxEnergy->SetLabelWidth(80);
   fMaxEnergy->Build();
   fMaxEnergy->SetLimits(0, 5000, 500001, TGNumberFormat::kNESRealOne);
   fMaxEnergy->SetToolTip("Maximum energy of displayed track.");
   fMaxEnergy->SetValue(5000);
   fMaxEnergy->Connect("ValueSet(Double_t)", "R3BEventManagerEditor",this, "MaxEnergy()");
   title1->AddFrame(fMaxEnergy, new TGLayoutHints(kLHintsTop, 1, 1, 1, 0));

   TGTextButton *fUpdate = new TGTextButton(title1, "Update");
   title1->AddFrame(fUpdate, new TGLayoutHints(kLHintsRight | kLHintsExpandX, 5,5,1,1));
   fUpdate->Connect("Clicked()", "R3BEventManagerEditor", this, "SelectEvent()");

   fInfoFrame->AddFrame(title1, new TGLayoutHints(kLHintsTop, 0, 0, 2, 0));

   fScaleE = new TGCheckButton(title1, "Scale by energy");
   AddFrame(fScaleE, new TGLayoutHints(kLHintsTop, 3, 1, 1, 0));
   fScaleE->Connect("Toggled(Bool_t)", "R3BEventManagerEditor", this, "DoScaleE()");
   
   title1->AddFrame(fScaleE);
   
   fManager->SetMaxEnergy(5000);

}

void R3BEventManagerEditor::MaxEnergy()
{
  fManager->SetMaxEnergy(fMaxEnergy->GetValue());

}

void R3BEventManagerEditor::SelectPDG()
{
 fManager->SelectPDG(fCurrentPDG->GetIntNumber());
} 
void R3BEventManagerEditor::SelectEvent()
{
  fManager->GotoEvent(fCurrentEvent->GetIntNumber()); 
  
  fMinEnergy->SetLimits(fManager->GetEvtMinEnergy(), fManager->GetEvtMaxEnergy(), 100);
  fMinEnergy->SetValue(fManager->GetMinEnergy());
  fMaxEnergy->SetLimits(fManager->GetEvtMinEnergy(), fManager->GetEvtMaxEnergy(), 100);
  fMaxEnergy->SetValue( fManager->GetMaxEnergy());
  
  Update();
}



void R3BEventManagerEditor::DoScaleE()
{
  if (fScaleE->IsOn()) ((R3BEventManager*) fManager)->SetScaleByEnergy(kTRUE);
  else  ((R3BEventManager*) fManager)->SetScaleByEnergy(kFALSE);
}
