#include "opcutils.h"
#include "wxLogin.h"
#include "simpleJSON/JSON.h"

#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/process.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/txtstrm.h>

using namespace std;
//(*InternalHeaders(wxLogin)




//(*IdInit(wxLogin)










BEGIN_EVENT_TABLE(wxLogin,wxDialog)
	//(*EventTable(wxLogin)
	//*)
END_EVENT_TABLE()

wxLogin::wxLogin(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(wxLogin)

















	this->txtPassword->SetFocus();
}

wxLogin::~wxLogin()
{
	//(*Destroy(wxLogin)
	//*)
}

void wxLogin::ExecuteLogin()
{
    LoginSuccessful = false;

    if (   this->txtUsername->GetValue() == ""
        || this->txtPassword->GetValue() == "" )
    {
        ::wxMessageBox("Username and Password are required.", "Login Error");
        return;
    }

    JSONValue *jsonValue = 0;
    if (OPCUtils::GithubConnect(this->txtUsername->GetValue(), this->txtPassword->GetValue(),
                                "https://api.github.com/",
                                &jsonValue))
    {
        JSONObject root = jsonValue->AsObject();
        if (   root.find(L"current_user_url") != root.end()
            && root[L"current_user_url"]->IsString())
        {
            LoginSuccessful = true;
            cout << "current_user_url: " << (root[L"current_user_url"]->AsString().c_str()) << "\n";
            wxMessageBox("Access granted.", "Github Login");
        }
    }

    if (jsonValue != 0)
        delete jsonValue;

    if (!LoginSuccessful)
    {
        wxMessageBox("Access denied.", "Github Login");
        this->txtPassword->SetFocus();
    }
    else
        this->EndModal(wxID_OK);
}

void wxLogin::OnbtnOKClick(wxCommandEvent& event)
{
    ExecuteLogin();
}

void wxLogin::OnbtnCancelClick(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}

void wxLogin::OntxtPasswordTextEnter(wxCommandEvent& event)
{
    ExecuteLogin();
}