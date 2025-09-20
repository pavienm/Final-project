#include <wx/wx.h>

class FareFrame : public wxFrame {
public:
    FareFrame()
    : wxFrame(nullptr, wxID_ANY, "Fare Calculator", wxDefaultPosition, wxSize(640, 460)) {
        SetMinSize(wxSize(640, 460));

        wxPanel* panel = new wxPanel(this);
        auto* root = new wxBoxSizer(wxVERTICAL);

        auto* title = new wxStaticText(panel, wxID_ANY, "Fare Calculator");
        title->SetFont(title->GetFont().MakeBold().Scale(1.2));
        root->Add(title, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 12);

        auto* form = new wxFlexGridSizer(0, 2, 10, 12);
        form->AddGrowableCol(1, 1);
        root->Add(form, 0, wxEXPAND | wxLEFT | wxRIGHT, 16);

        panel->SetSizer(root); // apply layout
    }
};

class FareApp : public wxApp {
public:
    bool OnInit() override {
        auto* f = new FareFrame();
        f->Centre();
        f->Show();
        return true;
    }
};
wxIMPLEMENT_APP(FareApp);
