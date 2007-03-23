/*
    Copyright (c) 2007 by Duncan Mac-Vicar Prett   <duncan@kde.org>
    
    *************************************************************************
    *                                                                       *
    * This program is free software; you can redistribute it and/or modify  *
    * it under the terms of the GNU General Public License as published by  *
    * the Free Software Foundation; either version 2 of the License, or     *
    * (at your option) any later version.                                   *
    *                                                                       *
    *************************************************************************
*/

#ifndef ZYppUpdaterPreferencesModule_H
#define ZYppUpdaterPreferencesModule_H

#include <kcmodule.h>
#include <qstring.h>

class ZYppUpdaterPreferences_Base;

/**
  *@author Duncan Mac-Vicar Prett
  */

class ZYppUpdaterPreferencesModule : public KCModule
{
  Q_OBJECT
public:

  ZYppUpdaterPreferencesModule(QWidget *parent = 0, const QStringList &args = QStringList());
  ~ZYppUpdaterPreferencesModule();

  virtual void save();
  virtual void load();

private:
  ZYppUpdaterPreferences_Base *m_preferencesWidget;
private slots:
  void slotModified();
};

#endif