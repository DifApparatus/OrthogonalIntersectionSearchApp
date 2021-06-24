#include <wx/wx.h>
#include <wx/event.h>
#include <vector>
#include <list>

struct Section{
    public:

    wxPoint point1;
    wxPoint point2;

    Section(wxPoint p1, wxPoint p2){
        point1 = p1;
        point2 = p2;
    }
    bool IsVertical(){
        return (point1.x == point2.x);
    }

};

class IntersectionSearchApp : public wxFrame
{
    public:
        IntersectionSearchApp(const wxString& title);

    private:
        std::vector<Section> sectionArray;
        std::list<Section> sectionsToDraw;
        wxButton *drawButton;
        wxPanel *field;

        int const horizontalRes = 1920;
        int const verticalRes = 1080;

        int const fieldLength = 1800;
        int const fieldHeight = 900;

        int const desiredSectionQuantity = 50000;

        void GenerateSectionArray();
        void IntersectionScan();
        void OnPaint(wxPaintEvent& event);
        void OnButtonPressed(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()
};

bool SectionEdgeSort(std::pair<wxPoint,Section> edge1, std::pair<wxPoint,Section> edge2);

