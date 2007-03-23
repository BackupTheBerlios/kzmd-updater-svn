#include <qlayout.h>

#include <kparts/componentfactory.h>
#include <klocale.h>
#include <kgenericfactory.h>
#include <kdebug.h>
#include <knuminput.h>
#include "zyppupdaterpreferences_base.h"

typedef KGenericFactory<ZYppUpdaterPreferencesModule> ZYppUpdaterPreferencesModuleFactory;
K_EXPORT_COMPONENT_FACTORY( kcm_kopete_latex, ZYppUpdaterPreferencesModuleFactory( "kcm_opensuseupdater_zypp" )  )

ZYppUpdaterPreferencesModule::ZYppUpdaterPreferencesModule(QWidget *parent, const QStringList &args)
              : KCModule(ZYppUpdaterPreferencesModuleFactory::componentData(), parent, args)
{
  QVBoxLayout* l = new QVBoxLayout( this );
  QWidget* w = new QWidget;
  m_preferencesWidget = new ZYppUpdaterPreferences_Base;
  m_preferencesWidget->setupUi( w );
  l->addWidget( w );

  // connect widget signals here
  m_preferencesWidget->horizontalDPI->setMinimum(1);
  m_preferencesWidget->verticalDPI->setMinimum(1);
  
  connect(m_preferencesWidget->horizontalDPI, SIGNAL(valueChanged(int)), this, SLOT(slotModified()));
  connect(m_preferencesWidget->verticalDPI, SIGNAL(valueChanged(int)), this, SLOT(slotModified()));
  
  load();
}

ZYppUpdaterPreferencesModule::~ZYppUpdaterPreferencesModule()
{
  delete m_preferencesWidget;
}

void ZYppUpdaterPreferencesModule::load()
{
  //LatexConfig::self()->readConfig();
  // load widgets here
  //m_preferencesWidget->horizontalDPI->setValue(LatexConfig::self()->horizontalDPI());
  //m_preferencesWidget->verticalDPI->setValue(LatexConfig::self()->verticalDPI());
  emit KCModule::changed(false);
}

void ZYppUpdaterPreferencesModule::slotModified()
{
  emit KCModule::changed(true);
}

void ZYppUpdaterPreferencesModule::save()
{
  //LatexConfig::self()->setHorizontalDPI(m_preferencesWidget->horizontalDPI->value());
  //LatexConfig::self()->setVerticalDPI(m_preferencesWidget->verticalDPI->value());
  //LatexConfig::self()->writeConfig();
  emit KCModule::changed(false);
}

#include "ZYppUpdaterPreferencesModule.moc"

// vim: set noet ts=4 sts=4 sw=4:
