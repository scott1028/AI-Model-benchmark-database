// rea decompile_all
// name: FUN_00114570
// address: 0x114570
// body: available
// range: 0x114570-0x114605 inclusive
// range: 0x114610-0x115654 inclusive
// range: 0x115658-0x115672 inclusive
// range: 0x115678-0x115692 inclusive
// range: 0x115698-0x1156b2 inclusive
// range: 0x1156b8-0x1156e7 inclusive
// range: 0x1156f0-0x115707 inclusive
// range: 0x115710-0x115744 inclusive
// range: 0x115748-0x115762 inclusive
// range: 0x115768-0x115782 inclusive
// range: 0x115788-0x1157ba inclusive
// range: 0x1157c0-0x1157da inclusive
// range: 0x1157e0-0x1157fa inclusive
// range: 0x115800-0x115817 inclusive
// range: 0x115820-0x115837 inclusive
// range: 0x115840-0x115857 inclusive
// range: 0x115860-0x115877 inclusive
// range: 0x115880-0x115897 inclusive
// range: 0x1158a0-0x1158b7 inclusive
// range: 0x1158c0-0x1158d7 inclusive
// range: 0x1158e0-0x1158f7 inclusive
// range: 0x115900-0x115917 inclusive
// range: 0x115920-0x115954 inclusive
// range: 0x115958-0x115974 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x115650 to 0x10f170 <EXTERNAL>::QWidget::show (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x115970 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00114570(QWidget *param_1)

{
  long *plVar1;
  long lVar2;
  QArrayData *pQVar3;
  wchar16 *pwVar4;
  char cVar5;
  uint uVar6;
  KConfigDialog *this;
  KCoreConfigSkeleton *pKVar7;
  QObject *pQVar8;
  QVBoxLayout *this_00;
  QCheckBox *pQVar9;
  QHBoxLayout *this_01;
  QLabel *pQVar10;
  QComboBox *this_02;
  undefined8 *puVar11;
  QString *pQVar12;
  undefined8 uVar13;
  QGridLayout *this_03;
  KPluralHandlingSpinBox *pKVar14;
  undefined4 *puVar15;
  bool bVar16;
  long in_FS_OFFSET;
  ulong uVar17;
  Connection local_100 [8];
  code *local_f8;
  undefined8 uStack_f0;
  undefined1 local_e8 [16];
  undefined8 local_d8;
  undefined1 local_c8 [16];
  undefined8 local_b8;
  QArrayData *local_a8;
  wchar16 *pwStack_a0;
  undefined8 local_98;
  undefined1 local_88 [16];
  long local_78;
  QVariant local_68 [16];
  undefined8 local_58;
  undefined8 local_50;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  local_78 = 8;
  local_88 = ZEXT816(0x1222d6) << 0x40;
  cVar5 = KConfigDialog::showDialog((QString *)local_88);
  if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
    UNLOCK();
    if (*(int *)local_88._0_8_ == 0) {
      QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
    }
  }
  if (cVar5 == '\0') {
    this = operator_new(0x38);
    pKVar7 = (KCoreConfigSkeleton *)FUN_0011f4c0();
    local_78 = 8;
    local_88 = ZEXT816(0x1222d6) << 0x40;
    KConfigDialog::KConfigDialog(this,param_1,(QString *)local_88,pKVar7);
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    local_e8 = (undefined1  [16])0x0;
    local_c8 = ZEXT816(0x122638) << 0x40;
    local_d8 = 0;
    local_b8 = 0x14;
    ki18nc(local_88,"@title:tab");
    KLocalizedString::toString();
    KLocalizedString::~KLocalizedString((KLocalizedString *)local_88);
    pQVar8 = operator_new(0x70);
    QWidget::QWidget((QWidget *)pQVar8,this,0);
    *(undefined ***)pQVar8 = &PTR_metaObject_00127628;
    *(undefined ***)(pQVar8 + 0x10) = &PTR_FUN_001277d8;
    QObject::objectName();
    lVar2 = local_78;
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    if (lVar2 == 0) {
      QObject::setObjectName(pQVar8,"GeneralOptsConfig",0x11);
    }
    local_88._0_8_ = 0x870000013c;
    QWidget::resize(pQVar8);
    this_00 = operator_new(0x20);
    QVBoxLayout::QVBoxLayout(this_00,(QWidget *)pQVar8);
    *(QVBoxLayout **)(pQVar8 + 0x28) = this_00;
    QObject::setObjectName(this_00,"verticalLayout",0xe);
    pQVar9 = operator_new(0x28);
    QCheckBox::QCheckBox(pQVar9,(QWidget *)pQVar8);
    *(QCheckBox **)(pQVar8 + 0x30) = pQVar9;
    QObject::setObjectName(pQVar9,"kcfg_UseQuestionMarks",0x15);
    QBoxLayout::addWidget(*(undefined8 *)(pQVar8 + 0x28),*(undefined8 *)(pQVar8 + 0x30),0,0);
    pQVar9 = operator_new(0x28);
    QCheckBox::QCheckBox(pQVar9,(QWidget *)pQVar8);
    *(QCheckBox **)(pQVar8 + 0x38) = pQVar9;
    QObject::setObjectName(pQVar9,"kcfg_AllowKminesReset",0x15);
    QAbstractButton::setChecked(SUB81(*(undefined8 *)(pQVar8 + 0x38),0));
    QCheckBox::setTristate(SUB81(*(undefined8 *)(pQVar8 + 0x38),0));
    QBoxLayout::addWidget(*(undefined8 *)(pQVar8 + 0x28),*(undefined8 *)(pQVar8 + 0x38),0,0);
    pQVar9 = operator_new(0x28);
    QCheckBox::QCheckBox(pQVar9,(QWidget *)pQVar8);
    *(QCheckBox **)(pQVar8 + 0x40) = pQVar9;
    QObject::setObjectName(pQVar9,"kcfg_DisableScoreOnReset",0x18);
    QBoxLayout::addWidget(*(undefined8 *)(pQVar8 + 0x28),*(undefined8 *)(pQVar8 + 0x40),0,0);
    pQVar9 = operator_new(0x28);
    QCheckBox::QCheckBox(pQVar9,(QWidget *)pQVar8);
    *(QCheckBox **)(pQVar8 + 0x48) = pQVar9;
    QObject::setObjectName(pQVar9,"kcfg_ExploreWithLeftClickOnNumberCells",0x26);
    QBoxLayout::addWidget(*(undefined8 *)(pQVar8 + 0x28),*(undefined8 *)(pQVar8 + 0x48),0,0);
    this_01 = operator_new(0x20);
    QHBoxLayout::QHBoxLayout(this_01);
    *(QHBoxLayout **)(pQVar8 + 0x50) = this_01;
    QObject::setObjectName(this_01,"horizontalLayout",0x10);
    pQVar10 = operator_new(0x28);
    QLabel::QLabel(pQVar10,pQVar8,0);
    *(QLabel **)(pQVar8 + 0x58) = pQVar10;
    QObject::setObjectName(pQVar10,"label",5);
    QBoxLayout::addWidget(*(undefined8 *)(pQVar8 + 0x50),*(undefined8 *)(pQVar8 + 0x58),0,0);
    this_02 = operator_new(0x28);
    QComboBox::QComboBox(this_02,(QWidget *)pQVar8);
    *(QComboBox **)(pQVar8 + 0x60) = this_02;
    local_68[0] = (QVariant)0x0;
    local_68[1] = (QVariant)0x0;
    local_68[2] = (QVariant)0x0;
    local_68[3] = (QVariant)0x0;
    local_68[4] = (QVariant)0x0;
    local_68[5] = (QVariant)0x0;
    local_68[6] = (QVariant)0x0;
    local_68[7] = (QVariant)0x0;
    local_68[8] = (QVariant)0x0;
    local_68[9] = (QVariant)0x0;
    local_68[10] = (QVariant)0x0;
    local_68[0xb] = (QVariant)0x0;
    local_68[0xc] = (QVariant)0x0;
    local_68[0xd] = (QVariant)0x0;
    local_68[0xe] = (QVariant)0x0;
    local_68[0xf] = (QVariant)0x0;
    local_88 = (undefined1  [16])0x0;
    local_58 = 0;
    local_50 = 2;
    local_78 = 0;
    uVar6 = QComboBox::count();
    QIcon::QIcon((QIcon *)&local_f8);
    QComboBox::insertItem
              ((int)this_02,(QIcon *)(ulong)uVar6,(QString *)&local_f8,(QVariant *)local_88);
    QIcon::~QIcon((QIcon *)&local_f8);
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    QVariant::~QVariant(local_68);
    uVar13 = *(undefined8 *)(pQVar8 + 0x60);
    local_58 = 0;
    local_68[0] = (QVariant)0x0;
    local_68[1] = (QVariant)0x0;
    local_68[2] = (QVariant)0x0;
    local_68[3] = (QVariant)0x0;
    local_68[4] = (QVariant)0x0;
    local_68[5] = (QVariant)0x0;
    local_68[6] = (QVariant)0x0;
    local_68[7] = (QVariant)0x0;
    local_68[8] = (QVariant)0x0;
    local_68[9] = (QVariant)0x0;
    local_68[10] = (QVariant)0x0;
    local_68[0xb] = (QVariant)0x0;
    local_68[0xc] = (QVariant)0x0;
    local_68[0xd] = (QVariant)0x0;
    local_68[0xe] = (QVariant)0x0;
    local_68[0xf] = (QVariant)0x0;
    local_88 = (undefined1  [16])0x0;
    local_50 = 2;
    local_78 = 0;
    uVar6 = QComboBox::count();
    QIcon::QIcon((QIcon *)&local_f8);
    QComboBox::insertItem
              ((int)uVar13,(QIcon *)(ulong)uVar6,(QString *)&local_f8,(QVariant *)local_88);
    QIcon::~QIcon((QIcon *)&local_f8);
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    QVariant::~QVariant(local_68);
    QObject::setObjectName(*(undefined8 *)(pQVar8 + 0x60),"kcfg_PlaceFlagOn",0x10);
    QBoxLayout::addWidget(*(undefined8 *)(pQVar8 + 0x50),*(undefined8 *)(pQVar8 + 0x60),0,0);
    QBoxLayout::addLayout(*(QLayout **)(pQVar8 + 0x28),(int)*(undefined8 *)(pQVar8 + 0x50));
    puVar11 = operator_new(0x28);
    plVar1 = *(long **)(pQVar8 + 0x28);
    *puVar11 = 0x126e10;
    *(undefined4 *)((long)puVar11 + 0x14) = 0x710000;
    lVar2 = *plVar1;
    *(undefined4 *)(puVar11 + 1) = 0;
    *(undefined8 *)((long)puVar11 + 0xc) = 0x2800000014;
    *(undefined8 **)(pQVar8 + 0x68) = puVar11;
    puVar11[3] = 0;
    puVar11[4] = 0xffffffffffffffff;
    (**(code **)(lVar2 + 0x80))(plVar1,puVar11);
    pQVar12 = *(QString **)(pQVar8 + 0x30);
    ki18n((char *)&local_f8);
    KLocalizedString::toString();
    KLocalizedString::~KLocalizedString((KLocalizedString *)&local_f8);
    QAbstractButton::setText(pQVar12);
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    pQVar12 = *(QString **)(pQVar8 + 0x38);
    ki18n((char *)&local_f8);
    KLocalizedString::toString();
    KLocalizedString::~KLocalizedString((KLocalizedString *)&local_f8);
    QAbstractButton::setText(pQVar12);
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    pQVar12 = *(QString **)(pQVar8 + 0x40);
    ki18n((char *)&local_f8);
    KLocalizedString::toString();
    KLocalizedString::~KLocalizedString((KLocalizedString *)&local_f8);
    QAbstractButton::setText(pQVar12);
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    pQVar12 = *(QString **)(pQVar8 + 0x48);
    ki18n((char *)&local_f8);
    KLocalizedString::toString();
    KLocalizedString::~KLocalizedString((KLocalizedString *)&local_f8);
    QAbstractButton::setText(pQVar12);
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    pQVar12 = *(QString **)(pQVar8 + 0x58);
    ki18n((char *)&local_f8);
    KLocalizedString::toString();
    KLocalizedString::~KLocalizedString((KLocalizedString *)&local_f8);
    QLabel::setText(pQVar12);
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    uVar13 = *(undefined8 *)(pQVar8 + 0x60);
    ki18n((char *)&local_f8);
    KLocalizedString::toString();
    KLocalizedString::~KLocalizedString((KLocalizedString *)&local_f8);
    QComboBox::setItemText((int)uVar13,(QString *)0x0);
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    uVar13 = *(undefined8 *)(pQVar8 + 0x60);
    ki18n((char *)&local_f8);
    KLocalizedString::toString();
    KLocalizedString::~KLocalizedString((KLocalizedString *)&local_f8);
    QComboBox::setItemText((int)uVar13,(QString *)0x1);
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    QMetaObject::connectSlotsByName(pQVar8);
    bVar16 = SUB81((KLocalizedString *)local_e8,0);
    KConfigDialog::addPage
              ((QWidget *)this,(QString *)pQVar8,(QString *)&local_a8,(QString *)local_c8,bVar16);
    if (local_a8 != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_a8 = *(int *)local_a8 + -1;
      UNLOCK();
      if (*(int *)local_a8 == 0) {
        QArrayData::deallocate(local_a8,2,0x10);
      }
    }
    if ((QArrayData *)local_c8._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_c8._0_8_ = *(int *)local_c8._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_c8._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_c8._0_8_,2,0x10);
      }
    }
    if ((QArrayData *)local_e8._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_e8._0_8_ = *(int *)local_e8._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_e8._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_e8._0_8_,2,0x10);
      }
    }
    local_c8 = (undefined1  [16])0x0;
    local_a8 = (QArrayData *)0x0;
    pwStack_a0 = u_games_config_theme_00122610;
    local_b8 = 0;
    local_98 = 0x12;
    ki18nc((char *)local_e8,"@title:tab");
    KLocalizedString::toString();
    KLocalizedString::~KLocalizedString((KLocalizedString *)local_e8);
    pQVar12 = operator_new(0x30);
    uVar13 = KGameRenderer::themeProvider();
    KGameThemeSelector::KGameThemeSelector((KGameThemeSelector *)pQVar12,uVar13,0,0);
    KConfigDialog::addPage
              ((QWidget *)this,pQVar12,(QString *)local_88,(QString *)&local_a8,
               SUB81((QString *)local_c8,0));
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    if (local_a8 != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_a8 = *(int *)local_a8 + -1;
      UNLOCK();
      if (*(int *)local_a8 == 0) {
        QArrayData::deallocate(local_a8,2,0x10);
      }
    }
    if ((QArrayData *)local_c8._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_c8._0_8_ = *(int *)local_c8._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_c8._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_c8._0_8_,2,0x10);
      }
    }
    local_e8 = (undefined1  [16])0x0;
    local_c8 = ZEXT816(0x1225e8) << 0x40;
    local_d8 = 0;
    local_b8 = 0x13;
    ki18nc(local_88,"@title:tab");
    KLocalizedString::toString();
    KLocalizedString::~KLocalizedString((KLocalizedString *)local_88);
    pQVar8 = operator_new(0x70);
    QWidget::QWidget((QWidget *)pQVar8,this,0);
    *(undefined ***)pQVar8 = &PTR_FUN_00127428;
    *(undefined ***)(pQVar8 + 0x10) = &PTR_FUN_001275d8;
    QObject::objectName();
    lVar2 = local_78;
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    if (lVar2 == 0) {
      QObject::setObjectName(pQVar8,"CustomGameConfig",0x10);
    }
    local_88._0_8_ = &DAT_f10000013c;
    QWidget::resize(pQVar8);
    this_03 = operator_new(0x20);
    QGridLayout::QGridLayout(this_03,(QWidget *)pQVar8);
    *(QGridLayout **)(pQVar8 + 0x28) = this_03;
    QObject::setObjectName(this_03,"gridLayout",10);
    pQVar10 = operator_new(0x28);
    QLabel::QLabel(pQVar10,pQVar8,0);
    *(QLabel **)(pQVar8 + 0x30) = pQVar10;
    QObject::setObjectName(pQVar10,"label",5);
    uVar17 = 0;
    QGridLayout::addWidget(*(undefined8 *)(pQVar8 + 0x28),*(undefined8 *)(pQVar8 + 0x30),0,0,1,1,0);
    pKVar14 = operator_new(0x30);
    KPluralHandlingSpinBox::KPluralHandlingSpinBox(pKVar14,(QWidget *)pQVar8);
    *(KPluralHandlingSpinBox **)(pQVar8 + 0x38) = pKVar14;
    QObject::setObjectName(pKVar14,"kcfg_CustomWidth",0x10);
    uVar17 = uVar17 & 0xffffffff00000000;
    QGridLayout::addWidget
              (*(undefined8 *)(pQVar8 + 0x28),*(undefined8 *)(pQVar8 + 0x38),0,1,1,1,uVar17);
    pQVar10 = operator_new(0x28);
    QLabel::QLabel(pQVar10,pQVar8,0);
    *(QLabel **)(pQVar8 + 0x40) = pQVar10;
    QObject::setObjectName(pQVar10,"label_2",7);
    uVar17 = uVar17 & 0xffffffff00000000;
    QGridLayout::addWidget
              (*(undefined8 *)(pQVar8 + 0x28),*(undefined8 *)(pQVar8 + 0x40),1,0,1,1,uVar17);
    pKVar14 = operator_new(0x30);
    KPluralHandlingSpinBox::KPluralHandlingSpinBox(pKVar14,(QWidget *)pQVar8);
    *(KPluralHandlingSpinBox **)(pQVar8 + 0x48) = pKVar14;
    QObject::setObjectName(pKVar14,"kcfg_CustomHeight",0x11);
    uVar17 = uVar17 & 0xffffffff00000000;
    QGridLayout::addWidget
              (*(undefined8 *)(pQVar8 + 0x28),*(undefined8 *)(pQVar8 + 0x48),1,1,1,1,uVar17);
    pQVar10 = operator_new(0x28);
    QLabel::QLabel(pQVar10,pQVar8,0);
    *(QLabel **)(pQVar8 + 0x50) = pQVar10;
    QObject::setObjectName(pQVar10,"label_3",7);
    uVar17 = uVar17 & 0xffffffff00000000;
    QGridLayout::addWidget
              (*(undefined8 *)(pQVar8 + 0x28),*(undefined8 *)(pQVar8 + 0x50),2,0,1,1,uVar17);
    pKVar14 = operator_new(0x30);
    KPluralHandlingSpinBox::KPluralHandlingSpinBox(pKVar14,(QWidget *)pQVar8);
    *(KPluralHandlingSpinBox **)(pQVar8 + 0x58) = pKVar14;
    QObject::setObjectName(pKVar14,"kcfg_CustomMines",0x10);
    uVar17 = uVar17 & 0xffffffff00000000;
    QGridLayout::addWidget
              (*(undefined8 *)(pQVar8 + 0x28),*(undefined8 *)(pQVar8 + 0x58),2,1,1,1,uVar17);
    puVar11 = operator_new(0x28);
    uVar13 = *(undefined8 *)(pQVar8 + 0x28);
    *puVar11 = 0x126e10;
    *(undefined8 *)((long)puVar11 + 0xc) = 0x1400000028;
    puVar11[3] = 0;
    puVar11[4] = 0xffffffffffffffff;
    *(undefined4 *)(puVar11 + 1) = 0;
    *(undefined4 *)((long)puVar11 + 0x14) = 0x170000;
    *(undefined8 **)(pQVar8 + 0x60) = puVar11;
    uVar17 = uVar17 & 0xffffffff00000000;
    QGridLayout::addItem(uVar13,puVar11,0,2,1,1,uVar17);
    puVar11 = operator_new(0x28);
    uVar13 = *(undefined8 *)(pQVar8 + 0x28);
    *puVar11 = 0x126e10;
    *(undefined8 *)((long)puVar11 + 0xc) = 0x2800000014;
    *(undefined4 *)(puVar11 + 1) = 0;
    *(undefined4 *)((long)puVar11 + 0x14) = 0x710000;
    *(undefined8 **)(pQVar8 + 0x68) = puVar11;
    puVar11[3] = 0;
    puVar11[4] = 0xffffffffffffffff;
    QGridLayout::addItem(uVar13,puVar11,3,1,1,1,uVar17 & 0xffffffff00000000);
    pQVar12 = *(QString **)(pQVar8 + 0x30);
    ki18n((char *)&local_f8);
    KLocalizedString::toString();
    KLocalizedString::~KLocalizedString((KLocalizedString *)&local_f8);
    QLabel::setText(pQVar12);
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    pQVar12 = *(QString **)(pQVar8 + 0x40);
    ki18n((char *)&local_f8);
    KLocalizedString::toString();
    KLocalizedString::~KLocalizedString((KLocalizedString *)&local_f8);
    QLabel::setText(pQVar12);
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    pQVar12 = *(QString **)(pQVar8 + 0x50);
    ki18n((char *)&local_f8);
    KLocalizedString::toString();
    KLocalizedString::~KLocalizedString((KLocalizedString *)&local_f8);
    QLabel::setText(pQVar12);
    if ((QArrayData *)local_88._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_88._0_8_ = *(int *)local_88._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_88._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_88._0_8_,2,0x10);
      }
    }
    QMetaObject::connectSlotsByName(pQVar8);
    uVar13 = *(undefined8 *)(pQVar8 + 0x38);
    local_88._8_8_ = 0;
    local_88._0_8_ = QSpinBox::valueChanged;
    local_f8 = FUN_00113fc0;
    uStack_f0 = 0;
    puVar15 = operator_new(0x20);
    *puVar15 = 1;
    *(code **)(puVar15 + 2) = FUN_00110c10;
    *(code **)(puVar15 + 4) = local_f8;
    *(undefined8 *)(puVar15 + 6) = uStack_f0;
    QObject::connectImpl
              (local_100,uVar13,local_88,pQVar8,(QIcon *)&local_f8,puVar15,0,0,
               QSpinBox::staticMetaObject);
    QMetaObject::Connection::~Connection(local_100);
    uVar13 = *(undefined8 *)(pQVar8 + 0x48);
    local_f8 = FUN_00113fc0;
    uStack_f0 = 0;
    local_88._8_8_ = 0;
    local_88._0_8_ = QSpinBox::valueChanged;
    puVar15 = operator_new(0x20);
    *puVar15 = 1;
    *(code **)(puVar15 + 2) = FUN_00110c10;
    *(code **)(puVar15 + 4) = local_f8;
    *(undefined8 *)(puVar15 + 6) = uStack_f0;
    QObject::connectImpl
              (local_100,uVar13,local_88,pQVar8,(QIcon *)&local_f8,puVar15,0,0,
               QSpinBox::staticMetaObject);
    QMetaObject::Connection::~Connection(local_100);
    KConfigDialog::addPage
              ((QWidget *)this,(QString *)pQVar8,(QString *)&local_a8,(QString *)local_c8,bVar16);
    if (local_a8 != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_a8 = *(int *)local_a8 + -1;
      UNLOCK();
      if (*(int *)local_a8 == 0) {
        QArrayData::deallocate(local_a8,2,0x10);
      }
    }
    if ((QArrayData *)local_c8._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_c8._0_8_ = *(int *)local_c8._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_c8._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_c8._0_8_,2,0x10);
      }
    }
    if ((QArrayData *)local_e8._0_8_ != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_e8._0_8_ = *(int *)local_e8._0_8_ + -1;
      UNLOCK();
      if (*(int *)local_e8._0_8_ == 0) {
        QArrayData::deallocate((QArrayData *)local_e8._0_8_,2,0x10);
      }
    }
    pwStack_a0 = (wchar16 *)0x0;
    local_a8 = (QArrayData *)FUN_001144f0;
    uVar13 = KGameRenderer::themeProvider();
    pwVar4 = pwStack_a0;
    pQVar3 = local_a8;
    local_88._8_8_ = 0;
    local_88._0_8_ = KGameThemeProvider::currentThemeChanged;
    puVar15 = operator_new(0x20);
    *(QArrayData **)(puVar15 + 4) = pQVar3;
    *(code **)(puVar15 + 2) = FUN_00110990;
    *puVar15 = 1;
    *(wchar16 **)(puVar15 + 6) = pwVar4;
    QObject::connectImpl
              ((QString *)local_c8,uVar13,local_88,param_1,(QString *)&local_a8,puVar15,0,0,
               KGameThemeProvider::staticMetaObject);
    QMetaObject::Connection::~Connection((Connection *)local_c8);
    pwStack_a0 = (wchar16 *)0x0;
    local_a8 = (QArrayData *)FUN_001144f0;
    local_88._8_8_ = 0;
    local_88._0_8_ = KConfigDialog::settingsChanged;
    puVar15 = operator_new(0x20);
    *puVar15 = 1;
    *(undefined8 *)(puVar15 + 6) = 0;
    *(code **)(puVar15 + 2) = FUN_00110990;
    *(code **)(puVar15 + 4) = FUN_001144f0;
    QObject::connectImpl
              ((QString *)local_c8,this,local_88,param_1,(QString *)&local_a8,puVar15,0,0,
               KConfigDialog::staticMetaObject);
    QMetaObject::Connection::~Connection((Connection *)local_c8);
    if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
      QWidget::show();
      return;
    }
  }
  else if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

