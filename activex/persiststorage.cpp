/*****************************************************************************
 * persiststorage.cpp: ActiveX control for VLC
 *****************************************************************************
 * Copyright (C) 2005 VideoLAN
 *
 * Authors: Damien Fouilleul <Damien.Fouilleul@laposte.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************/

#include "plugin.h"
#include "persiststorage.h"

using namespace std;

STDMETHODIMP VLCPersistStorage::GetClassID(LPCLSID pClsID)
{
    if( NULL == pClsID )
        return E_POINTER;

    *pClsID = _p_instance->getClassID();

    return S_OK;
};

STDMETHODIMP VLCPersistStorage::IsDirty(void)
{
    return S_FALSE;
};

STDMETHODIMP VLCPersistStorage::InitNew(IStorage *pStg)
{
    if( NULL == pStg )
        return E_POINTER;

    return _p_instance->onInit(TRUE);
};

STDMETHODIMP VLCPersistStorage::Load(IStorage *pStg)
{
    if( NULL == pStg )
        return E_POINTER;

    return _p_instance->onInit(TRUE);
};

STDMETHODIMP VLCPersistStorage::Save(IStorage *pStg, BOOL fSameAsLoad)
{
    if( NULL == pStg )
        return E_POINTER;

    return S_OK;
};

STDMETHODIMP VLCPersistStorage::SaveCompleted(IStorage *pStg)
{
    if( NULL == pStg )
        return E_POINTER;

    return S_OK;
};

STDMETHODIMP VLCPersistStorage::HandsOffStorage(void)
{
    return S_OK;
};

