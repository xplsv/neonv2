/**
 *  @file SSingleton.h Copyright 2005 DLE. All rights reserved. 
 *  @author Alejandro López
 *  @brief  Base class for singletons
 *
 */

#ifndef _DLE_COMMON_SINGLETON_SSINGLETON_H_
#define _DLE_COMMON_SINGLETON_SSINGLETON_H_

#ifndef _DLE_STATIC_DATA_ALLOWED_
#include "common/singleton/CSingletonMgr.h"
#endif

namespace DLE
{
    /**
     *  Base class for singletons
     */
    template <class S, int ID=-1, class I = S>
    class SSingleton
    #ifndef _DLE_STATIC_DATA_ALLOWED_
        : public MSingleton
    #endif
    {
    public:

        static I* GetInstance ()
        {
    #ifdef _DLE_STATIC_DATA_ALLOWED_
            static S gSingleton;
            return &gSingleton;
    #else
            S* s = static_cast<S*> ( DLE_SINGLETONMANAGER_GETSINGLETON ( ID ) );

            if ( !s )
            {
                s = new S;
                CSingletonMgr::GetInstance ()->RegisterSingleton ( s, ID );
            }

            return s;
    #endif
        }

    protected:

        SSingleton ()                           {}
        SSingleton ( SSingleton const& )        {}
        void operator= ( SSingleton const& )    {}
        ~SSingleton ()                          {}
    };
}

#endif //_DLE_COMMON_SINGLETON_SSINGLETON_H_
