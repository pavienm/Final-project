#include <wx/wx.h>
#include <wx/valnum.h>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>

// --- Green theme helpers ---
class GreenCheckBox : public wxCheckBox {
public:
    GreenCheckBox(wxWindow* parent, wxWindowID id, const wxString& label)
        : wxCheckBox(parent, id, label) {
        SetForegroundColour(wxColour(0, 100, 0));     // dark green text
    }
};

class GreenTextCtrl : public wxTextCtrl {
public:
    GreenTextCtrl(wxWindow* parent, wxWindowID id, const wxString& value, long style = 0)
        : wxTextCtrl(parent, id, value, wxDefaultPosition, wxDefaultSize, style) {
        SetForegroundColour(wxColour(0, 100, 0));     // green text
        SetBackgroundColour(wxColour(240, 255, 240)); // light green bg
    }
};

// --- Data structure for car variants ---
struct CarVariant {
    wxString model;
    wxString variant;
    int      engine_cc;
    double   km_per_l;
    double   rate_per_km;
};

// --- Main window ---
class FareFrame : public wxFrame {
public:
    FareFrame()
    : wxFrame(nullptr, wxID_ANY, "Fare Calculator", wxDefaultPosition, wxSize(640, 500)) {
        SetMinSize(wxSize(640, 500));
        InitCarData();

        // Root panel & sizer
        wxPanel* panel = new wxPanel(this);
        auto* root = new wxBoxSizer(wxVERTICAL);
        panel->SetSizer(root);
        panel->SetBackgroundColour(wxColour(235, 250, 235)); // soft green

        // Title
        auto* title = new wxStaticText(panel, wxID_ANY, "Fare Calculator (RM) - with Car Models");
        title->SetFont(title->GetFont().MakeBold().Scale(1.2));
        title->SetForegroundColour(wxColour(0, 100, 0));
        root->Add(title, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 12);

        // 2-column form
        auto* form = new wxFlexGridSizer(0, 2, 10, 12);
        form->AddGrowableCol(1, 1);
        root->Add(form, 0, wxEXPAND | wxLEFT | wxRIGHT, 16);

        // Model / Variant
        AddLabeledField(panel, form, "Model", modelChoice_);
        for (const auto& m : modelsOrder_) modelChoice_->Append(m);
        modelChoice_->SetSelection(0);

        AddLabeledField(panel, form, "Variant", variantChoice_);

        // Info labels (tinted green)
        form->Add(new wxStaticText(panel, wxID_ANY, "Rate per km (RM)"), 0, wxALIGN_CENTER_VERTICAL);
        rateLabel_ = new wxStaticText(panel, wxID_ANY, "RM0.00");
        rateLabel_->SetFont(rateLabel_->GetFont().MakeBold());
        rateLabel_->SetForegroundColour(wxColour(0, 128, 0));
        form->Add(rateLabel_, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

        form->Add(new wxStaticText(panel, wxID_ANY, "Engine (cc)"), 0, wxALIGN_CENTER_VERTICAL);
        engineLabel_ = new wxStaticText(panel, wxID_ANY, "-");
        engineLabel_->SetForegroundColour(wxColour(0, 128, 0));
        form->Add(engineLabel_, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

        form->Add(new wxStaticText(panel, wxID_ANY, "Fuel Consumption (km/L)"), 0, wxALIGN_CENTER_VERTICAL);
        kmlLabel_ = new wxStaticText(panel, wxID_ANY, "-");
        kmlLabel_->SetForegroundColour(wxColour(0, 128, 0));
        form->Add(kmlLabel_, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

        // Inputs (green widgets)
        form->Add(new wxStaticText(panel, wxID_ANY, "Distance (km)"), 0, wxALIGN_CENTER_VERTICAL);
        distInput_ = new GreenTextCtrl(panel, wxID_ANY, "", wxTE_RIGHT);
        { wxFloatingPointValidator<double> v(2, nullptr, wxNUM_VAL_DEFAULT); v.SetMin(0.0); distInput_->SetValidator(v); }
        form->Add(distInput_, 1, wxEXPAND);

        form->Add(new wxStaticText(panel, wxID_ANY, "Time (minutes)"), 0, wxALIGN_CENTER_VERTICAL);
        timeInput_ = new GreenTextCtrl(panel, wxID_ANY, "", wxTE_RIGHT);
        { wxIntegerValidator<unsigned int> v(nullptr, wxNUM_VAL_DEFAULT); timeInput_->SetValidator(v); }
        form->Add(timeInput_, 1, wxEXPAND);

        // Options (green checkboxes)
        form->Add(new wxStaticText(panel, wxID_ANY, "Traffic jam (+20%)"), 0, wxALIGN_CENTER_VERTICAL);
        trafficChk_ = new GreenCheckBox(panel, wxID_ANY, "");
        form->Add(trafficChk_, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

        form->Add(new wxStaticText(panel, wxID_ANY, "Night charge (+30%)"), 0, wxALIGN_CENTER_VERTICAL);
        nightChk_ = new GreenCheckBox(panel, wxID_ANY, "");
        form->Add(nightChk_, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

        // Buttons (green background + white text)
        root->AddSpacer(8);
        auto* btns = new wxBoxSizer(wxHORIZONTAL);
        root->Add(btns, 0, wxEXPAND | wxLEFT | wxRIGHT, 16);
        btns->AddStretchSpacer(1);
        auto* calcBtn = new wxButton(panel, wxID_ANY, "Calculate Fare");
        calcBtn->SetBackgroundColour(wxColour(0, 128, 0));
        calcBtn->SetForegroundColour(*wxWHITE);
        auto* clearBtn = new wxButton(panel, wxID_ANY, "Clear");
        clearBtn->SetBackgroundColour(wxColour(34, 139, 34));
        clearBtn->SetForegroundColour(*wxWHITE);
        btns->Add(calcBtn, 0); btns->AddSpacer(8); btns->Add(clearBtn, 0);

        // Result + note (green result)
        root->AddSpacer(10);
        resultText_ = new wxStaticText(panel, wxID_ANY, "Fare: RM0.00");
        resultText_->SetFont(resultText_->GetFont().MakeBold().Scale(1.3));
        resultText_->SetForegroundColour(wxColour(0, 100, 0));
        root->Add(resultText_, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 12);

        auto* note = new wxStaticText(panel, wxID_ANY,
            "Formula: base = (Distance x Rate) + (Time x RM0.10), then apply + 20% (jam) and/or + 30% (night)");
        note->Wrap(560);
        note->SetForegroundColour(wxColour(60, 120, 60));
        root->Add(note, 0, wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxRIGHT | wxBOTTOM, 12);

        // Events
        modelChoice_->Bind(wxEVT_CHOICE, &FareFrame::OnModelChanged, this);
        variantChoice_->Bind(wxEVT_CHOICE, &FareFrame::OnVariantChanged, this);
        calcBtn->Bind(wxEVT_BUTTON, &FareFrame::OnCalculate, this);
        clearBtn->Bind(wxEVT_BUTTON, &FareFrame::OnClear, this);

        // Initialize dropdowns/info
        PopulateVariantsForSelectedModel();
        UpdateVariantInfo();
    }

private:
    // --- UI elements ---
    wxChoice*     modelChoice_{nullptr};
    wxChoice*     variantChoice_{nullptr};
    wxStaticText* rateLabel_{nullptr};
    wxStaticText* engineLabel_{nullptr};
    wxStaticText* kmlLabel_{nullptr};
    GreenTextCtrl*   distInput_{nullptr};
    GreenTextCtrl*   timeInput_{nullptr};
    GreenCheckBox*   trafficChk_{nullptr};
    GreenCheckBox*   nightChk_{nullptr};
    wxStaticText* resultText_{nullptr};

    // --- Data ---
    std::vector<CarVariant> variants_;
    std::map<wxString, std::vector<int>> modelToIndices_;
    std::vector<wxString> modelsOrder_;

    // --- Helpers ---
    void AddLabeledField(wxPanel* p, wxFlexGridSizer* form, const wxString& label, wxChoice*& choice) {
        auto* lbl = new wxStaticText(p, wxID_ANY, label);
        lbl->SetForegroundColour(wxColour(0, 100, 0));
        form->Add(lbl, 0, wxALIGN_CENTER_VERTICAL);

        choice = new wxChoice(p, wxID_ANY);
        choice->SetForegroundColour(wxColour(0, 100, 0));
        choice->SetBackgroundColour(wxColour(245, 255, 245));
        form->Add(choice, 1, wxEXPAND);
    }

    void InitCarData() {
        auto add = [&](const wxString& model, const wxString& variant, int cc, double kml, double rate){
            int idx = static_cast<int>(variants_.size());
            variants_.push_back({model, variant, cc, kml, rate});
            modelToIndices_[model].push_back(idx);
        };

        // Bezza
        add("Perodua Bezza", "1.0 G (M)",  998, 22.8, 1.20);
        add("Perodua Bezza", "1.3 X (A)", 1329, 21.0, 1.20);
        add("Perodua Bezza", "1.3 AV (A)",1329, 22.0, 1.20);
        // Saga
        add("Proton Saga", "1.3 Standard M/T", 1297, 14.0, 1.20);
        add("Proton Saga", "1.3 Standard A/T", 1297, 14.0, 1.20);
        add("Proton Saga", "1.3 Ace A/T",      1297, 13.5, 1.20);
        add("Proton Saga", "1.3 R3 M/T",       1332, 14.0, 1.20);
        add("Proton Saga", "1.3 R3 A/T",       1332, 14.0, 1.20);
        // Persona
        add("Proton Persona", "1.6 Standard CVT", 1597, 15.2, 1.50);
        add("Proton Persona", "1.6 Premium CVT",  1597, 12.3, 1.50);
        // Vios
        add("Toyota Vios", "1.3 XLE CVT", 1329, 15.0, 1.50);
        // Myvi
        add("Perodua Myvi", "1.3 G (M)", 1297, 15.4, 1.20);
        add("Perodua Myvi", "1.5 X (A)", 1495, 17.5, 1.20);

        modelsOrder_ = {"Perodua Bezza","Proton Saga","Proton Persona","Toyota Vios","Perodua Myvi"};
    }

    int CurrentVariantIndex() const {
        wxString model = modelChoice_->GetStringSelection();
        int vsel = variantChoice_->GetSelection();
        if (!modelToIndices_.count(model) || vsel < 0) return -1;
        const auto& idxs = modelToIndices_.at(model);
        if (vsel >= static_cast<int>(idxs.size())) return -1;
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
        int idx = CurrentVariantIndex();
        if (idx < 0) return;
        const auto& cv = variants_[idx];
        rateLabel_->SetLabel(wxString::Format("RM%.2f", cv.rate_per_km));
        engineLabel_->SetLabel(wxString::Format("%d", cv.engine_cc));
        kmlLabel_->SetLabel(wxString::Format("%.1f", cv.km_per_l));
    }

    bool ReadInputs(double& distanceKm, unsigned int& timeMin) {
        if (!Validate() || !TransferDataFromWindow()) return false;
        wxString ds = distInput_->GetValue().Trim().Trim(false);
        wxString ts = timeInput_->GetValue().Trim().Trim(false);
        if (ds.empty() || ts.empty()) return false;
        double d; long t;
        if (!ds.ToDouble(&d) || d < 0) return false;
        if (!ts.ToLong(&t) || t < 0) return false;
        distanceKm = d; timeMin = static_cast<unsigned>(t);
        return true;
    }

    static wxString FormatRM(double amount) {
        std::ostringstream oss; oss.setf(std::ios::fixed); oss << std::setprecision(2) << amount;
        return wxString::Format("RM%s", oss.str());
    }

    // --- Events ---
    void OnModelChanged(wxCommandEvent&) { PopulateVariantsForSelectedModel(); UpdateVariantInfo(); }
    void OnVariantChanged(wxCommandEvent&) { UpdateVariantInfo(); }

    void OnCalculate(wxCommandEvent&) {
        double dist = 0.0; unsigned tmin = 0;
        if (!ReadInputs(dist, tmin)) {
            wxMessageBox("Please enter valid Distance (>=0) and Time (>=0).",
                         "Invalid input", wxICON_WARNING | wxOK, this);
            return;
        }
        int idx = CurrentVariantIndex();
        if (idx < 0) {
            wxMessageBox("Please select a model and variant.", "Missing selection",
                         wxICON_WARNING | wxOK, this);
            return;
        }
        const auto& cv = variants_[idx];
        double fare = (dist * cv.rate_per_km) + (static_cast<double>(tmin) * 0.10);
        if (trafficChk_->GetValue()) fare *= 1.20;
        if (nightChk_->GetValue())   fare *= 1.30;
        resultText_->SetLabel(wxString::Format("Fare: %s", FormatRM(fare)));
    }

    void OnClear(wxCommandEvent&) {
        modelChoice_->SetSelection(0);
        PopulateVariantsForSelectedModel();
        UpdateVariantInfo();
        distInput_->Clear();
        timeInput_->Clear();
        trafficChk_->SetValue(false);
        nightChk_->SetValue(false);
        resultText_->SetLabel("Fare: RM0.00");
    }
};

// --- App bootstrap ---
class FareApp : public wxApp {
public:
    bool OnInit() override {
        if (!wxApp::OnInit()) return false;
        auto* frame = new FareFrame();
        frame->Centre();
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(FareApp);
