// rea decompile_all
// name: FUN_0010fc00
// address: 0x10fc00
// body: available
// range: 0x10fc00-0x110570 inclusive
// range: 0x110578-0x1106c2 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x1106be to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 FUN_0010fc00(int param_1,char **param_2)

{
  char cVar1;
  undefined4 uVar2;
  char *pcVar3;
  size_t sVar4;
  long lVar5;
  void *pvVar6;
  int iVar7;
  long in_FS_OFFSET;
  int local_1dc [3];
  KAboutData local_1d0 [8];
  QApplication local_1c8 [16];
  undefined8 local_1b8;
  wchar16 *pwStack_1b0;
  undefined8 local_1a8;
  undefined1 local_198 [16];
  undefined8 local_188;
  undefined8 local_178;
  wchar16 *pwStack_170;
  undefined8 local_168;
  undefined1 local_158 [16];
  undefined8 local_148;
  undefined1 local_138 [16];
  undefined8 local_128;
  undefined1 local_118 [32];
  QUrl local_f8 [32];
  QUrl local_d8 [16];
  undefined8 local_c8;
  undefined1 local_b8 [16];
  undefined8 local_a8;
  QString local_98 [16];
  undefined8 local_88;
  undefined8 local_78;
  wchar16 *pwStack_70;
  undefined8 local_68;
  undefined1 local_58 [16];
  long local_48;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  local_1dc[0] = param_1;
  KIconTheme::initTheme();
  QApplication::QApplication(local_1c8,local_1dc,param_2,0x60a02);
  KStyleManager::initStyle();
  local_48 = 6;
  local_58 = ZEXT816(0x1221d5) << 0x40;
  KLocalizedString::setApplicationDomain((QByteArray *)local_58);
  if ((QArrayData *)local_58._0_8_ != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_58._0_8_ = *(int *)local_58._0_8_ + -1;
    UNLOCK();
    if (*(int *)local_58._0_8_ == 0) {
      QArrayData::deallocate((QArrayData *)local_58._0_8_,1,0x10);
    }
  }
  local_1b8 = 0;
  pwStack_1b0 = L"submit@bugs.kde.org";
  local_178 = 0;
  pwStack_170 = L"https://apps.kde.org/kmines";
  local_198 = (undefined1  [16])0x0;
  local_158 = (undefined1  [16])0x0;
  local_138 = (undefined1  [16])0x0;
  local_188 = 0;
  local_1a8 = 0x13;
  local_148 = 0;
  local_168 = 0x1b;
  local_128 = 0;
  FUN_00113f60(local_118,
               "(c) 1996-2005, Nicolas Hadacek\n(c) 2001, Mikhail Kourinny\n(c) 2006-2007, Mauricio Piacentini\n(c) 2007, Dmitry Suzdalev"
              );
  FUN_00113f60(local_f8,"KMines is a classic minesweeper game");
  local_b8 = (undefined1  [16])0x0;
  local_d8 = (QUrl  [16])(ZEXT816(0x122468) << 0x40);
  local_a8 = 0;
  local_c8 = 9;
  FUN_00113f60(local_98,"KMines");
  local_58 = (undefined1  [16])0x0;
  local_78 = 0;
  pwStack_70 = L"kmines";
  local_48 = 0;
  local_68 = 6;
  KAboutData::KAboutData
            (local_1d0,&local_78,local_98,local_d8,local_f8,1,local_118,local_138,&local_178,
             &local_1b8);
  FUN_00116630(&local_78);
  FUN_00116630((QByteArray *)local_58);
  FUN_00116630(local_98);
  FUN_00116630(local_d8);
  FUN_00116630(local_b8);
  FUN_00116630(local_f8);
  FUN_00116630(local_118);
  FUN_00116630(local_138);
  FUN_00116630(&local_178);
  FUN_00116630(local_158);
  FUN_00116630(&local_1b8);
  FUN_00116630(local_198);
  QUrl::QUrl(local_f8);
  local_d8[0] = (QUrl)0x0;
  local_d8[1] = (QUrl)0x0;
  local_d8[2] = (QUrl)0x0;
  local_d8[3] = (QUrl)0x0;
  local_d8[4] = (QUrl)0x0;
  local_d8[5] = (QUrl)0x0;
  local_d8[6] = (QUrl)0x0;
  local_d8[7] = (QUrl)0x0;
  local_d8[8] = (QUrl)0x0;
  local_d8[9] = (QUrl)0x0;
  local_d8[10] = (QUrl)0x0;
  local_d8[0xb] = (QUrl)0x0;
  local_d8[0xc] = (QUrl)0x0;
  local_d8[0xd] = (QUrl)0x0;
  local_d8[0xe] = (QUrl)0x0;
  local_d8[0xf] = (QUrl)0x0;
  local_b8._0_8_ = 0;
  local_b8._8_8_ = L"hadacek@kde.org";
  local_98[0] = (QString)0x0;
  local_98[1] = (QString)0x0;
  local_98[2] = (QString)0x0;
  local_98[3] = (QString)0x0;
  local_98[4] = (QString)0x0;
  local_98[5] = (QString)0x0;
  local_98[6] = (QString)0x0;
  local_98[7] = (QString)0x0;
  local_98[8] = (QString)0x0;
  local_98[9] = (QString)0x0;
  local_98[10] = (QString)0x0;
  local_98[0xb] = (QString)0x0;
  local_98[0xc] = (QString)0x0;
  local_98[0xd] = (QString)0x0;
  local_98[0xe] = (QString)0x0;
  local_98[0xf] = (QString)0x0;
  local_c8 = 0;
  local_88 = 0;
  local_a8 = 0xf;
  FUN_00113f60(&local_78,"Original author");
  FUN_00113f60((QByteArray *)local_58,"Nicolas Hadacek");
  KAboutData::addAuthor
            ((QString *)local_1d0,(QString *)local_58,(QString *)&local_78,(QString *)local_b8,
             local_d8);
  FUN_00116630((QByteArray *)local_58);
  FUN_00116630(&local_78);
  FUN_00116630(local_b8);
  FUN_00116630(local_98);
  FUN_00116630(local_d8);
  QUrl::~QUrl(local_f8);
  QUrl::QUrl(local_f8);
  local_d8[0] = (QUrl)0x0;
  local_d8[1] = (QUrl)0x0;
  local_d8[2] = (QUrl)0x0;
  local_d8[3] = (QUrl)0x0;
  local_d8[4] = (QUrl)0x0;
  local_d8[5] = (QUrl)0x0;
  local_d8[6] = (QUrl)0x0;
  local_d8[7] = (QUrl)0x0;
  local_d8[8] = (QUrl)0x0;
  local_d8[9] = (QUrl)0x0;
  local_d8[10] = (QUrl)0x0;
  local_d8[0xb] = (QUrl)0x0;
  local_d8[0xc] = (QUrl)0x0;
  local_d8[0xd] = (QUrl)0x0;
  local_d8[0xe] = (QUrl)0x0;
  local_d8[0xf] = (QUrl)0x0;
  local_b8._0_8_ = 0;
  local_b8._8_8_ = L"mauricio@tabuleiro.com";
  local_98[0] = (QString)0x0;
  local_98[1] = (QString)0x0;
  local_98[2] = (QString)0x0;
  local_98[3] = (QString)0x0;
  local_98[4] = (QString)0x0;
  local_98[5] = (QString)0x0;
  local_98[6] = (QString)0x0;
  local_98[7] = (QString)0x0;
  local_98[8] = (QString)0x0;
  local_98[9] = (QString)0x0;
  local_98[10] = (QString)0x0;
  local_98[0xb] = (QString)0x0;
  local_98[0xc] = (QString)0x0;
  local_98[0xd] = (QString)0x0;
  local_98[0xe] = (QString)0x0;
  local_98[0xf] = (QString)0x0;
  local_c8 = 0;
  local_88 = 0;
  local_a8 = 0x16;
  FUN_00113f60(&local_78,"Code refactoring and SVG support. Current maintainer");
  FUN_00113f60((QByteArray *)local_58,"Mauricio Piacentini");
  KAboutData::addAuthor
            ((QString *)local_1d0,(QString *)local_58,(QString *)&local_78,(QString *)local_b8,
             local_d8);
  FUN_00116630((QByteArray *)local_58);
  FUN_00116630(&local_78);
  FUN_00116630(local_b8);
  FUN_00116630(local_98);
  FUN_00116630(local_d8);
  QUrl::~QUrl(local_f8);
  QUrl::QUrl(local_f8);
  local_d8[0] = (QUrl)0x0;
  local_d8[1] = (QUrl)0x0;
  local_d8[2] = (QUrl)0x0;
  local_d8[3] = (QUrl)0x0;
  local_d8[4] = (QUrl)0x0;
  local_d8[5] = (QUrl)0x0;
  local_d8[6] = (QUrl)0x0;
  local_d8[7] = (QUrl)0x0;
  local_d8[8] = (QUrl)0x0;
  local_d8[9] = (QUrl)0x0;
  local_d8[10] = (QUrl)0x0;
  local_d8[0xb] = (QUrl)0x0;
  local_d8[0xc] = (QUrl)0x0;
  local_d8[0xd] = (QUrl)0x0;
  local_d8[0xe] = (QUrl)0x0;
  local_d8[0xf] = (QUrl)0x0;
  local_b8 = ZEXT816(0x122668) << 0x40;
  local_98[0] = (QString)0x0;
  local_98[1] = (QString)0x0;
  local_98[2] = (QString)0x0;
  local_98[3] = (QString)0x0;
  local_98[4] = (QString)0x0;
  local_98[5] = (QString)0x0;
  local_98[6] = (QString)0x0;
  local_98[7] = (QString)0x0;
  local_98[8] = (QString)0x0;
  local_98[9] = (QString)0x0;
  local_98[10] = (QString)0x0;
  local_98[0xb] = (QString)0x0;
  local_98[0xc] = (QString)0x0;
  local_98[0xd] = (QString)0x0;
  local_98[0xe] = (QString)0x0;
  local_98[0xf] = (QString)0x0;
  local_c8 = 0;
  local_88 = 0;
  local_a8 = 0x10;
  FUN_00113f60(&local_78,"Rewrite to use QGraphicsView framework. Current maintainer");
  FUN_00113f60((QByteArray *)local_58,"Dmitry Suzdalev");
  KAboutData::addAuthor
            ((QString *)local_1d0,(QString *)local_58,(QString *)&local_78,(QString *)local_b8,
             local_d8);
  FUN_00116630((QByteArray *)local_58);
  FUN_00116630(&local_78);
  FUN_00116630(local_b8);
  FUN_00116630(local_98);
  FUN_00116630(local_d8);
  QUrl::~QUrl(local_f8);
  QUrl::QUrl(local_d8);
  local_b8 = (undefined1  [16])0x0;
  local_98[0] = (QString)0x0;
  local_98[1] = (QString)0x0;
  local_98[2] = (QString)0x0;
  local_98[3] = (QString)0x0;
  local_98[4] = (QString)0x0;
  local_98[5] = (QString)0x0;
  local_98[6] = (QString)0x0;
  local_98[7] = (QString)0x0;
  local_98[8] = (QString)0x0;
  local_98[9] = (QString)0x0;
  local_98[10] = (QString)0x0;
  local_98[0xb] = (QString)0x0;
  local_98[0xc] = (QString)0x0;
  local_98[0xd] = (QString)0x0;
  local_98[0xe] = (QString)0x0;
  local_98[0xf] = (QString)0x0;
  local_a8 = 0;
  local_88 = 0;
  FUN_00113f60(&local_78,"Smiley pixmaps");
  FUN_00113f60((QByteArray *)local_58,"Andreas Zehender");
  KAboutData::addCredit
            ((QString *)local_1d0,(QString *)local_58,(QString *)&local_78,local_98,(QUrl *)local_b8
            );
  FUN_00116630((QByteArray *)local_58);
  FUN_00116630(&local_78);
  FUN_00116630(local_98);
  FUN_00116630(local_b8);
  QUrl::~QUrl(local_d8);
  QUrl::QUrl(local_d8);
  local_b8 = (undefined1  [16])0x0;
  local_98[0] = (QString)0x0;
  local_98[1] = (QString)0x0;
  local_98[2] = (QString)0x0;
  local_98[3] = (QString)0x0;
  local_98[4] = (QString)0x0;
  local_98[5] = (QString)0x0;
  local_98[6] = (QString)0x0;
  local_98[7] = (QString)0x0;
  local_98[8] = (QString)0x0;
  local_98[9] = (QString)0x0;
  local_98[10] = (QString)0x0;
  local_98[0xb] = (QString)0x0;
  local_98[0xc] = (QString)0x0;
  local_98[0xd] = (QString)0x0;
  local_98[0xe] = (QString)0x0;
  local_98[0xf] = (QString)0x0;
  local_a8 = 0;
  local_88 = 0;
  FUN_00113f60(&local_78,"Solver/Adviser");
  FUN_00113f60((QByteArray *)local_58,"Mikhail Kourinny");
  KAboutData::addCredit
            ((QString *)local_1d0,(QString *)local_58,(QString *)&local_78,local_98,(QUrl *)local_b8
            );
  FUN_00116630((QByteArray *)local_58);
  FUN_00116630(&local_78);
  FUN_00116630(local_98);
  FUN_00116630(local_b8);
  QUrl::~QUrl(local_d8);
  QUrl::QUrl(local_d8);
  local_b8 = (undefined1  [16])0x0;
  local_98[0] = (QString)0x0;
  local_98[1] = (QString)0x0;
  local_98[2] = (QString)0x0;
  local_98[3] = (QString)0x0;
  local_98[4] = (QString)0x0;
  local_98[5] = (QString)0x0;
  local_98[6] = (QString)0x0;
  local_98[7] = (QString)0x0;
  local_98[8] = (QString)0x0;
  local_98[9] = (QString)0x0;
  local_98[10] = (QString)0x0;
  local_98[0xb] = (QString)0x0;
  local_98[0xc] = (QString)0x0;
  local_98[0xd] = (QString)0x0;
  local_98[0xe] = (QString)0x0;
  local_98[0xf] = (QString)0x0;
  local_a8 = 0;
  local_88 = 0;
  FUN_00113f60(&local_78,"Magic reveal mode");
  FUN_00113f60((QByteArray *)local_58,"Thomas Capricelli");
  KAboutData::addCredit
            ((QString *)local_1d0,(QString *)local_58,(QString *)&local_78,local_98,(QUrl *)local_b8
            );
  FUN_00116630((QByteArray *)local_58);
  FUN_00116630(&local_78);
  FUN_00116630(local_98);
  FUN_00116630(local_b8);
  QUrl::~QUrl(local_d8);
  QUrl::QUrl(local_d8);
  local_b8 = (undefined1  [16])0x0;
  local_98[0] = (QString)0x0;
  local_98[1] = (QString)0x0;
  local_98[2] = (QString)0x0;
  local_98[3] = (QString)0x0;
  local_98[4] = (QString)0x0;
  local_98[5] = (QString)0x0;
  local_98[6] = (QString)0x0;
  local_98[7] = (QString)0x0;
  local_98[8] = (QString)0x0;
  local_98[9] = (QString)0x0;
  local_98[10] = (QString)0x0;
  local_98[0xb] = (QString)0x0;
  local_98[0xc] = (QString)0x0;
  local_98[0xd] = (QString)0x0;
  local_98[0xe] = (QString)0x0;
  local_98[0xf] = (QString)0x0;
  local_a8 = 0;
  local_88 = 0;
  FUN_00113f60(&local_78,"Port to use KGameRenderer");
  FUN_00113f60((QByteArray *)local_58,"Brian Croom");
  KAboutData::addCredit
            ((QString *)local_1d0,(QString *)local_58,(QString *)&local_78,local_98,(QUrl *)local_b8
            );
  FUN_00116630((QByteArray *)local_58);
  FUN_00116630(&local_78);
  FUN_00116630(local_98);
  FUN_00116630(local_b8);
  QUrl::~QUrl(local_d8);
  local_58 = (undefined1  [16])0x0;
  local_78 = 0;
  pwStack_70 = L"kmines";
  local_48 = 0;
  local_68 = 6;
  QIcon::fromTheme(local_98);
  QGuiApplication::setWindowIcon((QIcon *)local_98);
  QIcon::~QIcon((QIcon *)local_98);
  FUN_00116630(&local_78);
  FUN_00116630((QByteArray *)local_58);
  KAboutData::setApplicationData(local_1d0);
  KCrash::initialize();
  QCommandLineParser::QCommandLineParser((QCommandLineParser *)local_98);
  KAboutData::setupCommandLine((QCommandLineParser *)local_1d0);
  QCommandLineParser::process((QCoreApplication *)local_98);
  KAboutData::processCommandLine((QCommandLineParser *)local_1d0);
  KDBusService::KDBusService((KDBusService *)&local_78,2,0);
  cVar1 = QGuiApplication::isSessionRestored();
  if (cVar1 == '\0') {
    pvVar6 = operator_new(0x90);
    FUN_0011a610(pvVar6);
    QWidget::show();
  }
  else {
    iVar7 = 1;
    while( true ) {
      cVar1 = KMainWindow::canBeRestored(iVar7);
      if (cVar1 == '\0') break;
      KMainWindow::classNameOfToplevel((int)(QByteArray *)local_58);
      pcVar3 = (char *)QMetaObject::className();
      lVar5 = 0;
      if ((pcVar3 != (char *)0x0) && (*pcVar3 != '\0')) {
        sVar4 = strlen(pcVar3 + 1);
        lVar5 = sVar4 + 1;
      }
      if (lVar5 == local_48) {
        cVar1 = QtPrivate::equalStrings(lVar5,local_58._8_8_,lVar5,pcVar3);
        if (cVar1 != '\0') {
          pvVar6 = operator_new(0x90);
          FUN_0011a610(pvVar6);
          KMainWindow::restore((int)pvVar6,SUB41(iVar7,0));
        }
      }
      if ((QArrayData *)local_58._0_8_ != (QArrayData *)0x0) {
        LOCK();
        *(int *)local_58._0_8_ = *(int *)local_58._0_8_ + -1;
        UNLOCK();
        if (*(int *)local_58._0_8_ == 0) {
          QArrayData::deallocate((QArrayData *)local_58._0_8_,2,0x10);
        }
      }
      iVar7 = iVar7 + 1;
    }
  }
  uVar2 = QApplication::exec();
  KDBusService::~KDBusService((KDBusService *)&local_78);
  QCommandLineParser::~QCommandLineParser((QCommandLineParser *)local_98);
  KAboutData::~KAboutData(local_1d0);
  QApplication::~QApplication(local_1c8);
  if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
    return uVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

