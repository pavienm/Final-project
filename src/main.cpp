#include <wx/wx.h>
#include <wx/valnum.h>
#include <vector>
#include <map>

struct CarVariant { wxString model, variant; int engine_cc; double km_per_l, rate_per_km; };

class FareFrame : public wxFrame {
public:
    FareFrame()
    : wxFrame(nullptr, wxID_ANY, "Fare Calculator", wxDefaultPosition, wxSize(640, 460)) {
        SetMinSize(wxSize(640, 460));
        InitCarData();

        wxPanel* panel = new wxPanel(this);
        auto* root = new wxBoxSizer(wxVERTICAL);
        panel->SetSizer(root);

        auto* title = new wxStaticText(panel, wxID_ANY, "Fare Calculator (RM) - with Car Models");
        title->SetFont(title->GetFont().MakeBold().Scale(1.2));
        root->Add(title, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 12);

        auto* form = new wxFlexGridSizer(0, 2, 10, 12);
        form->AddGrowableCol(1, 1);
        root->Add(form, 0, wxEXPAND | wxLEFT | wxRIGHT, 16);

        AddLabeledField(panel, form, "Model", modelChoice_);
        for (const auto& m : modelsOrder_) modelChoice_->Append(m);
        modelChoice_->SetSelection(0);

        AddLabeledField(panel, form, "Variant", variantChoice_);

        form->Add(new wxStaticText(panel, wxID_ANY, "Rate per km (RM)"), 0, wxALIGN_CENTER_VERTICAL);
        rateLabel_ = new wxStaticText(panel, wxID_ANY, "RM0.00");
        rateLabel_->SetFont(rateLabel_->GetFont().MakeBold());
        root->Layout(); // optional nudge
        form->Add(rateLabel_, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

        form->Add(new wxStaticText(panel, wxID_ANY, "Engine (cc)"), 0, wxALIGN_CENTER_VERTICAL);
        engineLabel_ = new wxStaticText(panel, wxID_ANY, "-");
        form->Add(engineLabel_, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

        form->Add(new wxStaticText(panel, wxID_ANY, "Fuel Consumption (km/L)"), 0, wxALIGN_CENTER_VERTICAL);
        kmlLabel_ = new wxStaticText(panel, wxID_ANY, "-");
        form->Add(kmlLabel_, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

        modelChoice_->Bind(wxEVT_CHOICE, &FareFrame::OnModelChanged, this);
        variantChoice_->Bind(wxEVT_CHOICE, &FareFrame::OnVariantChanged, this);
        PopulateVariantsForSelectedModel();
        UpdateVariantInfo();
    }

private:
    wxChoice* modelChoice_{nullptr}; wxChoice* variantChoice_{nullptr};
    wxStaticText* rateLabel_{nullptr}; wxStaticText* engineLabel_{nullptr}; wxStaticText* kmlLabel_{nullptr};
    std::vector<CarVariant> variants_;
    std::map<wxString, std::vector<int>> modelToIndices_;
    std::vector<wxString> modelsOrder_;

    void AddLabeledField(wxPanel* p, wxFlexGridSizer* form, const wxString& label, wxChoice*& choice) {
        form->Add(new wxStaticText(p, wxID_ANY, label), 0, wxALIGN_CENTER_VERTICAL);
        choice = new wxChoice(p, wxID_ANY); form->Add(choice, 1, wxEXPAND);
    }
    void InitCarData() {
        auto add = [&](const wxString& model, const wxString& variant, int cc, double kml, double rate){
            int idx = (int)variants_.size();
            variants_.push_back({model, variant, cc, kml, rate});
            modelToIndices_[model].push_back(idx);
        };
        add("Perodua Bezza", "1.0 G (M)", 998, 22.8, 1.20);
        add("Perodua Bezza", "1.3 X (A)", 1329, 21.0, 1.20);
        add("Perodua Bezza", "1.3 AV (A)", 1329, 22.0, 1.20);
        add("Proton Saga", "1.3 Standard M/T", 1297, 14.0, 1.20);
        add("Proton Saga", "1.3 Standard A/T", 1297, 14.0, 1.20);
        add("Proton Saga", "1.3 Ace A/T", 1297, 13.5, 1.20);
        add("Proton Saga", "1.3 R3 M/T", 1332, 14.0, 1.20);
        add("Proton Saga", "1.3 R3 A/T", 1332, 14.0, 1.20);
        add("Proton Persona", "1.6 Standard CVT", 1597, 15.2, 1.50);
        add("Proton Persona", "1.6 Premium CVT", 1597, 12.3, 1.50);
        add("Toyota Vios", "1.3 XLE CVT", 1329, 15.0, 1.50);
        add("Perodua Myvi", "1.3 G (M)", 1297, 15.4, 1.20);
        add("Perodua Myvi", "1.5 X (A)", 1495, 17.5, 1.20);
        modelsOrder_ = {"Perodua Bezza","Proton Saga","Proton Persona","Toyota Vios","Perodua Myvi"};
    }
    int CurrentVariantIndex() const {
        wxString model = modelChoice_->GetStringSelection();
        int vsel = variantChoice_->GetSelection();
        if (!modelToIndices_.count(model) || vsel < 0) return -1;
        const auto& idxs = modelToIndices_.at(model);
        if (vsel >= (int)idxs.size()) return -1;
        return idxs[vsel];
    }
    void PopulateVariantsForSelectedModel() {
        variantChoice_->Clear();
        wxString model = modelChoice_->GetStringSelection();
        const auto& idxs = modelToIndices_[model];
        for (int i : idxs) variantChoice_->Append(variants_[i].variant);
        if (!idxs.empty()) variantChoice_->SetSelection(0);
    }
    void UpdateVariantInfo() {
        int idx = CurrentVariantIndex(); if (idx < 0) return;
        const auto& cv = variants_[idx];
        rateLabel_->SetLabel(wxString::Format("RM%.2f", cv.rate_per_km));
        engineLabel_->SetLabel(wxString::Format("%d", cv.engine_cc));
        kmlLabel_->SetLabel(wxString::Format("%.1f", cv.km_per_l));
    }
    void OnModelChanged(wxCommandEvent&) { PopulateVariantsForSelectedModel(); UpdateVariantInfo(); }
    void OnVariantChanged(wxCommandEvent&) { UpdateVariantInfo(); }
};

class FareApp : public wxApp {
public:
    bool OnInit() override {
        if (!wxApp::OnInit()) return false;
        auto* f = new FareFrame(); f->Centre(); f->Show();
        return true;
    }
};
wxIMPLEMENT_APP(FareApp);
