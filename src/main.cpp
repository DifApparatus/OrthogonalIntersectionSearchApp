#include "main.h"
#include "IntersectionSearchApp.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    IntersectionSearchApp *app = new IntersectionSearchApp(wxT("Orthogonal Intersections Search App"));
    app->Show(true);

    return true;
}

