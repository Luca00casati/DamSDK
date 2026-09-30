#include "DropTarget.h"

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {
    // FUNCTION: DELAYLAMA 0x10008690
    DropTarget* createDropTarget(Window* frame) {
        return new DropTarget(frame);
    }

    // FUNCTION: DELAYLAMA 0x100086f0
    DropTarget::DropTarget(Window* frame) {
        this->refCount = 0;
        this->parentFrame = frame;
    }

    // FUNCTION: DELAYLAMA 0x10008730
    DropTarget::~DropTarget() {}

    // FUNCTION: DELAYLAMA 0x10008740
    STDMETHODIMP DropTarget::QueryInterface(REFIID riid, void** object) {
        if (riid == IID_IDropTarget || riid == IID_IUnknown) {
            *object = this;
            AddRef();
            return NOERROR;
        }
        *object = 0;
        return E_NOINTERFACE;
    }

    // FUNCTION: DELAYLAMA 0x100087a0
    STDMETHODIMP_(ULONG) DropTarget::AddRef(void) {
        return ++this->refCount;
    }

    // FUNCTION: DELAYLAMA 0x100087b0
    STDMETHODIMP_(ULONG) DropTarget::Release(void) {
        this->refCount--;
        if (this->refCount <= 0)
            delete this;
        return this->refCount;
    }

    // FUNCTION: DELAYLAMA 0x100087e0
    STDMETHODIMP DropTarget::DragEnter(IDataObject* dataObject, DWORD keyState, POINTL pt, DWORD* effect) {
        this->canAcceptDrop = false;
        if (dataObject) {
            FORMATETC formatTextDrop = {CF_TEXT, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
            if (S_OK == dataObject->QueryGetData(&formatTextDrop)) {
                this->canAcceptDrop = true;
                return DragOver(keyState, pt, effect);
            }
            FORMATETC formatHDrop = {CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
            if (S_OK == dataObject->QueryGetData(&formatHDrop)) {
                this->canAcceptDrop = true;
                return DragOver(keyState, pt, effect);
            }
        }
        *effect = DROPEFFECT_NONE;
        return S_OK;
    }

    // FUNCTION: DELAYLAMA 0x100088c0
    STDMETHODIMP DropTarget::DragOver(DWORD keyState, POINTL pt, DWORD* effect) {
        if (this->canAcceptDrop) {
            if (keyState & MK_CONTROL)
                *effect = DROPEFFECT_COPY;
            else
                *effect = DROPEFFECT_MOVE;
        }
        else
            *effect = DROPEFFECT_NONE;
        return S_OK;
    }

    // FUNCTION: DELAYLAMA 0x10008900
    STDMETHODIMP DropTarget::DragLeave(void) {
        return S_OK;
    }

    // STUB: DELAYLAMA 0x10008910
    STDMETHODIMP DropTarget::Drop(IDataObject* dataObject, DWORD keyState, POINTL pt, DWORD* effect) {
        // char cVar1;
        // bool bVar2;
        // HRESULT HVar3;
        // SIZE_T SVar4;
        // UINT UVar5;
        // undefined4 *puVar6;
        // UINT UVar7;
        // char *pcVar8;
        // uint uVar9;
        // uint uVar10;
        // int iVar11;
        // char *pcVar12;
        // char *pcVar13;
        // LPVOID pvVar14;
        // LPVOID pvStack_450;
        // POINT PStack_44c;
        // STGMEDIUM local_444;
        // POINT PStack_438;
        // UINT UStack_430;
        // _union_2260 _Stack_42c;
        // FORMATETC local_428;
        // FORMATETC local_414;
        // CHAR aCStack_400 [1024];
        // LONG pathArray;
        //
        // if (this->parentFrame != (Window *)0x0) {
        //   local_428.lindex = -1;
        //   local_414.lindex = -1;
        //   local_428.cfFormat = 1;
        //   local_428.dwAspect = 1;
        //   local_428.tymed = 1;
        //   local_414.dwAspect = 1;
        //   local_414.tymed = 1;
        //   local_428.ptd = (DVTARGETDEVICE *)0x0;
        //   local_414.cfFormat = 0xf;
        //   local_414.ptd = (DVTARGETDEVICE *)0x0;
        //   bVar2 = false;
        //   HVar3 = (*pDataObj->lpVtbl->GetData)(pDataObj,&local_428,&local_444);
        //   if (HVar3 == 0) {
        //     bVar2 = true;
        //   }
        //   else {
        //     HVar3 = (*pDataObj->lpVtbl->GetData)(pDataObj,&local_414,&local_444);
        //     if (HVar3 != 0) goto LAB_10008b42;
        //   }
        //   _Stack_42c.hMetaFilePict = local_444.u.hMetaFilePict;
        //   if (local_444.u.hBitmap != (HBITMAP)0x0) {
        //     if (bVar2) {
        //       if (bVar2) {
        //         pvStack_450 = GlobalLock(local_444.u.hGlobal);
        //         SVar4 = GlobalSize(local_444.u.hGlobal);
        //         if ((pvStack_450 != (LPVOID)0x0) && (SVar4 != 0)) {
        //           PStack_44c.x = 0;
        //           PStack_44c.y = 0;
        //           Window::getLocalMousePos(this->parentFrame,&PStack_44c);
        //           (*(this->parentFrame->vtable->view).routeMessage)(&pvStack_450,SVar4,1,&PStack_44c);
        //         }
        //         GlobalUnlock(local_444.u.hGlobal);
        //         if (local_444.pUnkForRelease == (IUnknown *)0x0) {
        //           GlobalFree(local_444.u.hGlobal);
        //         }
        //         else {
        //           (*(local_444.pUnkForRelease)->lpVtbl->Release)(local_444.pUnkForRelease);
        //         }
        //       }
        //     }
        //     else {
        //       UVar5 = DragQueryFileA((HDROP)local_444.u.hBitmap,0xffffffff,(LPSTR)0x0,0);
        //       UStack_430 = UVar5;
        //       if (0 < (int)UVar5) {
        //         puVar6 = operator_new(UVar5 * 4);
        //         iVar11 = 0;
        //         pvStack_450 = (LPVOID)0x0;
        //         PStack_44c.x = (LONG)puVar6;
        //         if (0 < (int)UVar5) {
        //           do {
        //             pvVar14 = pvStack_450;
        //             UVar7 = DragQueryFileA((HDROP)_Stack_42c.hBitmap,(UINT)pvStack_450,aCStack_400,0x400);
        //             if (UVar7 != 0) {
        //               resolveShortcutTarget();
        //               pcVar8 = operator_new(0x400);
        //               uVar9 = 0xffffffff;
        //               *puVar6 = pcVar8;
        //               pcVar12 = aCStack_400;
        //               do {
        //                 pcVar13 = pcVar12;
        //                 if (uVar9 == 0) break;
        //                 uVar9 = uVar9 - 1;
        //                 pcVar13 = pcVar12 + 1;
        //                 cVar1 = *pcVar12;
        //                 pcVar12 = pcVar13;
        //               } while (cVar1 != '\0');
        //               uVar9 = ~uVar9;
        //               pcVar12 = pcVar13 + -uVar9;
        //               for (uVar10 = uVar9 >> 2; uVar10 != 0; uVar10 = uVar10 - 1) {
        //                 *(undefined4 *)pcVar8 = *(undefined4 *)pcVar12;
        //                 pcVar12 = pcVar12 + 4;
        //                 pcVar8 = pcVar8 + 4;
        //               }
        //               iVar11 = iVar11 + 1;
        //               for (uVar9 = uVar9 & 3; uVar9 != 0; uVar9 = uVar9 - 1) {
        //                 *pcVar8 = *pcVar12;
        //                 pcVar12 = pcVar12 + 1;
        //                 pcVar8 = pcVar8 + 1;
        //               }
        //               puVar6 = puVar6 + 1;
        //               UVar5 = UStack_430;
        //               pvVar14 = pvStack_450;
        //             }
        //             pvStack_450 = (LPVOID)((int)pvVar14 + 1);
        //           } while ((int)pvStack_450 < (int)UVar5);
        //         }
        //         PStack_438.x = 0;
        //         PStack_438.y = 0;
        //         Window::getLocalMousePos(this->parentFrame,&PStack_438);
        //         pathArray = PStack_44c.x;
        //         (*(this->parentFrame->vtable->view).routeMessage)(PStack_44c.x,UVar5,0,&PStack_438);
        //         puVar6 = (undefined4 *)pathArray;
        //         if (0 < iVar11) {
        //           do {
        //             operator_delete((void *)*puVar6);
        //             iVar11 = iVar11 + -1;
        //             puVar6 = puVar6 + 1;
        //           } while (iVar11 != 0);
        //         }
        //         operator_delete((void *)pathArray);
        //       }
        //     }
        //   }
        // }
        // LAB_10008b42:
        // (*this->vtable->returnZero)(this);
        // return;
        return S_OK;
    }

    // STUB: DELAYLAMA 0x10008b60
    HRESULT  DropTarget::resolveShortcutTarget() {
        // char *_Str1;
        // uint uVar1;
        // HRESULT HVar2;
        // int *unaff_ESI;
        // int *unaff_retaddr;
        // undefined4 in_stack_00001138;
        // char *in_stack_0000114c;
        // int *piVar3;
        // int *piVar4;
        //
        // __chkstk();
        // _Str1 = strrchr(in_stack_0000114c,0x2e);
        // uVar1 = 0;
        // if (_Str1 != (char *)0x0) {
        //   uVar1 = _stricmp(_Str1,(char *)&_Str2_1000d3f0);
        //   if (uVar1 == 0) {
        //     HVar2 = CoCreateInstance((IID *)&rclsid_1000c228,(LPUNKNOWN)0x0,1,(IID *)&riid_1000c238,
        //                              (LPVOID *)&stack0x00000000);
        //     if (-1 < HVar2) {
        //       piVar4 = (int *)&stack0x00000004;
        //       HVar2 = (**(code **)*unaff_retaddr)();
        //       if (-1 < HVar2) {
        //         MultiByteToWideChar(0,0,in_stack_0000114c,-1,(LPWSTR)&stack0x0000013c,0x800);
        //         piVar3 = (int *)0x0;
        //         HVar2 = (**(code **)(*unaff_ESI + 0x14))(unaff_ESI,&stack0x0000013c);
        //         if (-1 < HVar2) {
        //           HVar2 = (**(code **)(*piVar3 + 0x4c))(piVar3,0,0x1f40003);
        //           if (-1 < HVar2) {
        //             HVar2 = (**(code **)(*piVar3 + 0xc))
        //                               (piVar3,in_stack_00001138,0x800,&stack0xfffffff0,1);
        //           }
        //         }
        //         (**(code **)(*unaff_retaddr + 8))(unaff_retaddr);
        //       }
        //       (**(code **)(*piVar4 + 8))(piVar4);
        //     }
        //     return (uint)(-1 < HVar2);
        //   }
        // }
        // return uVar1 & 0xffffff00;
        return 0;
    }

    // STUB: DELAYLAMA 0x1000aa50
    void DropTarget::__chkstk() {
        // uint in_EAX;
        // undefined1 *puVar1;
        // undefined4 unaff_retaddr;
        //
        // puVar1 = &stack0x00000004;
        // for (; 0xfff < in_EAX; in_EAX = in_EAX - 0x1000) {
        //   puVar1 = puVar1 + -0x1000;
        // }
        // *(undefined4 *)(puVar1 + (-4 - in_EAX)) = unaff_retaddr;
        // return;
    }
}
}
}
}