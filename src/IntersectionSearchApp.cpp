#include "IntersectionSearchApp.h"
#include <wx/display.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <map>



BEGIN_EVENT_TABLE(IntersectionSearchApp, wxFrame)

EVT_PAINT(IntersectionSearchApp::OnPaint)

END_EVENT_TABLE()

IntersectionSearchApp::IntersectionSearchApp(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize())
{
    this->SetSize(horizontalRes,verticalRes);
    drawButton = new wxButton(this, wxEVT_BUTTON, "Draw Orthogonal Intersections",wxPoint(0,0),wxSize(200,30));
    field = new wxPanel(this, -1, wxPoint(50,50), wxSize(fieldLength,fieldHeight));
    drawButton->Bind(wxEVT_BUTTON, &IntersectionSearchApp::OnButtonPressed,this);
    GenerateSectionArray();
}

void IntersectionSearchApp::GenerateSectionArray()
{
    int **pointMatrix = new int*[fieldLength];
    for (int i=0;i<fieldLength;i++){
        pointMatrix[i] = new int[fieldHeight];
        for (int j=0;j<fieldHeight;j++){
            pointMatrix[i][j] = 0; // 0 - no sections, 1 - vertical section, 2 - horizontal section, 3 - both sections
        }
    }
    double prob = 10000/static_cast<double>(fieldLength*fieldHeight);
    int intersectionQuantity = 0;
    srand(time(NULL));
    while (intersectionQuantity < desiredSectionQuantity){
        int x1 = rand() % fieldLength;
        int y1 = rand() % fieldHeight;
        int length = 2 + rand() % 6;
        int direction = rand() % 2; // 0 - up, 1 - right
        if (direction == 0){
            int y2 = y1 + length - 1;
            int x2 = x1;
            if (y2 < fieldHeight && !(pointMatrix[x1][y1] & 1) && !(pointMatrix[x2][y2])){
                sectionArray.push_back(Section(wxPoint(x1,y1),wxPoint(x2,y2)));
                for (int i=0;i<length;i++){
                    pointMatrix[x1][y1+i] += 1;
                }
                intersectionQuantity++;
            }
            else continue;
        }
        if (direction == 1){
            int x2 = x1 + length - 1;
            int y2 = y1;
            if (x2 < fieldLength && !(pointMatrix[x1][y1] & 1) && !(pointMatrix[x2][y2])){
                sectionArray.push_back(Section(wxPoint(x1,y1),wxPoint(x2,y2)));
                for (int i=0;i<length;i++){
                    pointMatrix[x1+i][y1] += 2;
                }
                intersectionQuantity++;
            }
            else continue;
        }
    }
    return;
}
void IntersectionSearchApp::IntersectionScan()
{
    sectionsToDraw.clear();
    std::list<std::pair<wxPoint,Section>> sectionEdgeQueue;
    for (Section section: sectionArray)
    {
        if (section.point1.x == section.point2.x) {
                sectionEdgeQueue.push_back(std::make_pair(wxPoint(section.point1), section));
        }
        else {
            sectionEdgeQueue.push_back(std::make_pair(wxPoint(section.point1), section));
            sectionEdgeQueue.push_back(std::make_pair(wxPoint(section.point2), section));
        }
    }
    sectionEdgeQueue.sort(SectionEdgeSort);

    std::map<int,Section> sweepingLineMap;
    for (std::pair<wxPoint,Section>& sectionEdge : sectionEdgeQueue)
    {
        wxPoint point = std::get<0>(sectionEdge);
        Section section = std::get<1>(sectionEdge);
        if (section.IsVertical()){
            int y_min = section.point1.y;
            int y_max = section.point2.y;
            bool flag = 0;
            for (const auto &pair : sweepingLineMap) {
                if (pair.first >= y_min && pair.first <= y_max) {
                        sectionsToDraw.push_back(pair.second);
                        flag = 1;
                }
            }
            if (flag) sectionsToDraw.push_back(section);
        }
        else if (point == section.point1){
            sweepingLineMap.insert({point.y,section});
        }
        else {
            sweepingLineMap.erase(point.y);
        }
    }

}

void IntersectionSearchApp::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.SetPen( wxPen( wxColor(0,0,0), 1 ) );
    dc.SetLogicalOrigin(-50,-50);
    for (int i = 0; i < sectionArray.size(); i++){
        dc.DrawLine(sectionArray[i].point1, sectionArray[i].point2);
    }
    dc.SetPen(wxPen(wxColor(255,0,0), 1 ) );
    for (Section section : sectionsToDraw){
        dc.DrawLine(section.point1, section.point2);
    }
}

void IntersectionSearchApp::OnButtonPressed(wxCommandEvent& event){
    IntersectionScan();
    Refresh();
}

bool SectionEdgeSort(std::pair<wxPoint,Section> edge1, std::pair<wxPoint,Section> edge2)
{
    wxPoint point1 = std::get<0>(edge1);
    wxPoint point2 = std::get<0>(edge2);
    if (point1.x != point2.x) return (point1.x < point2.x);
    else {
        Section section1 = std::get<1>(edge1);
        Section section2 = std::get<1>(edge2);
        if (section1.IsVertical() && !section2.IsVertical()) return false;
        if (!section1.IsVertical() && section2.IsVertical()) return true;
        else return (point1.y < point2.y);
    }
}
