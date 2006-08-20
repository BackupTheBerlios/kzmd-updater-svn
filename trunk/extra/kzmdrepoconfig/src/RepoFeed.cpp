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

#include "RepoFeed.h"

#include <kurl.h>

RepoFeed::RepoFeed(QObject *parent) : QObject(parent) {
}

QString RepoFeed::url() {
	return rssUrl;
}
void RepoFeed::setUrl(const QString& serverUrl) {
	rssUrl = serverUrl;
}

void RepoFeed::fetch() {
	if (rssUrl.isEmpty())
		return;

	rssLoader = Loader::create();
	connect(rssLoader, SIGNAL(loadingComplete(Loader*, Document, Status)),
					this, SLOT(loadComplete(Loader*, Document, Status)));

	rssLoader->loadFrom(KURL(rssUrl), new FileRetriever);
}

void RepoFeed::loadComplete(Loader *ld, Document doc, Status stat) {
	if (stat == Success) {
		rssDocument = doc;
		parse();
	}
}

/*
	So, currently how are going to parse this is that each Article is a repo.

	The Article Title is the name of the feed, the Article Description the
	description and the Article Link is the actual feed link.

	WE ASSUME EVERYTHING IS ZYPP. 
*/

void RepoFeed::parse() {
	//List is typedef QValueList<Article>
	Article::List::iterator iter;
	Article::List articleList = rssDocument.articles();

	repoList.clear();

	for (iter = articleList.begin(); iter != articleList.end(); iter++) {
		Repo rep;
		rep.title = (*iter).title().simplifyWhiteSpace();
		rep.description = (*iter).description().simplifyWhiteSpace();
		rep.url = (*iter).link().prettyURL();
		repoList.append(rep);
	}
	emit(fetchDone());
}

QValueList<Repo> RepoFeed::repos() {
	return repoList;
}
