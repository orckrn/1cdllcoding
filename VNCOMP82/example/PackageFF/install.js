/**
 * See license.txt
 */
 
/******************************************************************************
 * Package Constants
 *****************************************************************************/ 
const NAME = "addin";  
const PKG_NAME = "AddInAdaptor";
const PKG_KEY = "/addin/" + NAME;
const PKG_VERSION = "1.0.0.0";

/******************************************************************************
 * Chrome Constants
 *****************************************************************************/
const CHROME_SPACE = 400;
const CHROME_LOCALES = ['en-US', 'bg-BG', 'ca-AD', 'cs-CZ', 'da-DK', 'de-DE', 'es-ES', 'fi-FI', 'fr-FR', 'hu-HU', 'it-IT', 'ko-KR', 'nb-NO', 'nl-NL', 'pl-PL', 'pt-BR', 'ro-RO', 'ru-RU', 'sk-SK', 'sv-SE', 'tr-TR', 'uk-UA', 'zh-CN'];
const CHROME_FOLDER = "chrome";
const CHROME_JAR = NAME + ".jar";
const CHROME_JARFOLDER = "chrome/" + CHROME_JAR;
const CHROME_CONTENT = "content/" + NAME + "/";
const CHROME_SKIN = "skin/classic/" + NAME + "/";
const CHROME_FLAGS = PROFILE_CHROME | DELAYED_CHROME;

/******************************************************************************
 * Defaults Constants
 *****************************************************************************/   
const DEFAULTS_SPACE = 50;
const DEFAULTS_FOLDER = "defaults";
const DEFAULTS_PREFS = "defaults/preferences/" + NAME + ".js"; 

/******************************************************************************
 * XPCOM Constants
 *****************************************************************************/   
const XPCOM_SPACE = 150;
const XPCOM_FOLDER = "components/";
const XPCOM_FILES = ["AddInFF.dll", "1CEAdnWeb.xpt", "AddInNative.dll"];
const XPCOM_REG = "components/.autoreg";

var success = performAll();
if (success)
{
    logComment("performInstall");
    Install.performInstall();
}
else
{
    logComment("cancelInstall");
    Install.cancelInstall(success);
}
      
/******************************************************************************
 * Function to do the install processing.
 *
 * @return    SUCCESS if install is successful.
 *****************************************************************************/   
function performAll()
{
    var code = initInstall(PKG_NAME, PKG_KEY, PKG_VERSION);
    code = performXPCOM()

    return true;
}
  
function performXPCOM()
{
    var folder = Install.getFolder("Components");
    
    for (var i = 0; i < XPCOM_FILES.length; i++)
    {
        code = Install.addFile(PKG_KEY, PKG_VERSION, 
                           XPCOM_FOLDER + XPCOM_FILES[i], 
                           folder, null);
    }

    //hack to make sure we register the component
    folder = Install.getFolder("Program");   
    code = Install.addFile(PKG_KEY, PKG_VERSION, XPCOM_REG, folder, null);
    code = registerComServer();
    
    return code;
}

function registerComServer()
{
    var folder = Install.getFolder("Components");

    var server = Install.getFolder("file:///", folder + "test.dll");
    code = File.windowsRegisterServer(server);            
    
    return code;
}

function verifyCode(code)
{
  //success code
  if (code == SUCCESS)
    return true;
  
  //reboot code
  if (code == REBOOT_NEEDED)
    return true;
    
  //error code
  return false; 
}