#include <iostream>
#include <unknwn.h>
#include "QI.h"

using namespace std;

// ����������
interface IX : IUnknown {
    virtual void _stdcall Fx() = 0;
};

interface IY : IUnknown {
    virtual void _stdcall Fy() = 0;
};

interface IZ : IUnknown {
    virtual void _stdcall Fz() = 0;
};

// ���������
class CA : public IX, public IY {
public:
    // ���������� ������� ������ IUnknown
    virtual ULONG _stdcall AddRef() { return 0; };
    virtual ULONG _stdcall Release() { return 0; };

    virtual HRESULT _stdcall QueryInterface(const IID& iid, void** ppv) {
        if (iid == IID_IUnknown1) {
            cout << "QueryInterface: ��������� ��������� �� IUnknown" << endl;
            *ppv = static_cast<IX*>(this);
        }
        else if (iid == IID_IX) {
            cout << "QueryInterface: ��������� ��������� �� IX" << endl;
            *ppv = static_cast<IX*>(this);
        }
        else if (iid == IID_IY) {
            cout << "QueryInterface: ��������� ��������� �� IY" << endl;
            *ppv = static_cast<IY*>(this);
        }
        else {
            cout << "��������� �� ��������������" << endl;
            *ppv = NULL;
            return E_NOINTERFACE;
        }
        reinterpret_cast<IUnknown*>(*ppv)->AddRef();
        return S_OK;
    }

    // ���������� ���������� IX
    virtual void _stdcall Fx() { std::cout << "CA::Fx" << std::endl; };

    // ���������� ���������� IY
    virtual void _stdcall Fy() { std::cout << "CA::Fy" << std::endl; };

};

// ������� �������� ����������
IUnknown* CreateInstance() {
    IUnknown* pI = static_cast<IX*>(new CA);
    pI->AddRef();
    return pI;
}


// ������
int main() {

    setlocale(LC_ALL, "Russian");
    cout << "Hello, world!" << endl;

    HRESULT hr;

    cout << "Client: get pointer to IUnknown" << endl;
    IUnknown* pIUnkonwn = CreateInstance();

    cout << "\nClient: get pointer to IX" << endl;
    IX* pIX = NULL;
    hr = pIUnkonwn->QueryInterface(IID_IX, (void**)&pIX);
    if (SUCCEEDED(hr)) {
        cout << "Client: IX received successfully" << endl;
        pIX->Fx(); // ������������ ��������� IX
    }

    cout << "\nClient: get pointer to IY" << endl;
    IY* pIY = NULL;
    hr = pIUnkonwn->QueryInterface(IID_IY, (void**)&pIY);
    if (SUCCEEDED(hr)) {
        cout << "Client: IY received successfully" << endl;
        pIY->Fy(); // ������������ ��������� IY
    };

    cout << "\nClient: Get unsupported interface" << endl;
    IZ* pIZ = NULL;
    hr = pIUnkonwn->QueryInterface(IID_IZ, (void**)&pIZ);
    if (SUCCEEDED(hr)) {
        cout << "Client: interface IZ get succesfully" << endl;
    }
    else {
        cout << "Client: Can not get interface IZ" << endl;
    }
    cout << "\n������: �������� ��������� �� IY ����� IX" << endl;
    IY* pIYfromIX = NULL;
    hr = pIX->QueryInterface(IID_IY, (void**)&pIYfromIX);
    if (SUCCEEDED(hr)) {
        cout << "������: ��������� �� IY ������� �������" << endl;
        pIYfromIX->Fy();
    }
    else {
        cout << "������: ���������� �������� ��������� �� IY ����� IX" << endl;
    };

    cout << "\n������: �������� ��������� �� IUnknown ����� IY" << endl;
    IUnknown* pIUnknownFromIY = NULL;
    hr = pIY->QueryInterface(IID_IUnknown1, (void**)&pIUnknownFromIY);
    if (SUCCEEDED(hr)) {
        cout << "����� ��� ���������?" << endl;
        if (pIUnknownFromIY == pIUnkonwn) {
            cout << "��" << endl;
        }
        else {
            cout << "HET" << endl;
        }
    };

    // ������� ���������
    delete pIUnkonwn;

    return 0;

}

