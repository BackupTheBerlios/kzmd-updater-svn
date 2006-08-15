/* 
   Copyright (C) 2006 Narayan Newton <narayannewton@gmail.com>

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
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef _REPO_FEED_H_
#define _REPO_FEED_H_

#include <qobject.h>
#include <rss/librss.h>

using namespace RSS;

struct Repo {
	QString title;
	QString description;
	QString url;
};

class RepoFeed : public QObject {

	Q_OBJECT

	public:

		RepoFeed(QObject *parent=0);

		QString url();
		void setUrl(const QString&);

		void fetch();

		QValueList<Repo> repos();

	signals:

		void fetchDone();

	private slots:

		void loadComplete(Loader *, Document, Status);

	private:

		void parse();

		QString rssUrl;
		Document rssDocument;
		Loader *rssLoader;
		QValueList<Repo> repoList;

};

#endif

