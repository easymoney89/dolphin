/* This file is part of the KDE project
   Copyright (C) 1998, 1999 David Faure <faure@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include "konq_misc.h"
#include "konq_mainview.h"
#include <kmessagebox.h>
#include <kurifilter.h>
#include <klocale.h>
#include <qdir.h>

/**********************************************
 *
 * KonqFileManager
 *
 **********************************************/

bool KonqFileManager::openFileManagerWindow( const KURL & _url )
{
  return openFileManagerWindow( _url, QString::null );
}

bool KonqFileManager::openFileManagerWindow( const KURL & _url, const QString &name )
{
  QList<KonqMainView> *mainViews = KonqMainView::mainViewList();
  if ( mainViews )
  {
    QListIterator<KonqMainView> it( *mainViews );
    for (; it.current(); ++it )
      if ( it.current()->fullScreenMode() )
        it.current()->slotFullScreenStop();
  }

  // If _url is 0L, open $HOME
  KURL url = !_url.isEmpty() ? _url : KURL(QDir::homeDirPath().prepend( "file:" ));

  KonqMainView *win = new KonqMainView( url );
  win->setInitialFrameName( name );
  win->show();

  return true; // why would it fail ? :)
}

QString konqFilteredURL( QWidget * parent, const QString &_url )
{
  KURIFilterData data = _url;
  KURIFilter::self()->filterURI( data );
  if( data.hasBeenFiltered() )
  {
    KURIFilterData::URITypes type = data.uriType();
    if( type == KURIFilterData::UNKNOWN )
    {
      KMessageBox::sorry( parent, i18n( "The url \"%1\" is of unknown type" ).arg( _url ) );
      return QString::null;  // should never happen unless the search filter is unavailable.
    }
    else if( type == KURIFilterData::ERROR )
    {
      KMessageBox::sorry( parent, i18n( data.errorMsg() ) );
      return QString::null;
    }
    else
      return data.uri().url();
  }
  return _url;  // return the original url if it cannot be filtered.
}

/**********************************************
 *
 * KonqBookmarkManager
 *
 **********************************************/

void KonqBookmarkManager::editBookmarks( const KURL & _url )
{
  // Will call the KonqFileManager re-implementation
  KFileManager::getFileManager()->openFileManagerWindow( _url );
}

