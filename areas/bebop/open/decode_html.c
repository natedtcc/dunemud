
// Decode HTML Entities
// Borrowed from /data/httpd/cgi-bin/status
// Written by Bubbs, April/2010.
// Modified by Bebop 08/03/2021

public string decode(string value);

public string decode(string value) {
  string output = "";
  foreach(string bit : regexplode(value, "&[a-zA-Z#0-9]+;")) {
    if (bit[0] == '&')
      switch(bit) {
        case "&quot;"  :  output += "\034";  break;
        case "&amp;"   :  output += "\038";  break;
        case "&apos;"  :  output += "\039";  break;
        case "&lt;"    :  output += "\060";  break;
        case "&gt;"    :  output += "\062";  break;
        case "&prime;" :  output += "′"; break;
        case "&Prime;" :  output += "″"; break;
        case "&nbsp;"  :  output += " "; break;
        case "&iexcl;" :  output += "¡"; break;
        case "&cent;"  :  output += "¢"; break;
        case "&pound;" :  output += "£"; break;
        case "&curren;":  output += "¤"; break;
        case "&yen;"   :  output += "¥"; break;
        case "&brvbar;":  output += "¦"; break;
        case "&sect;"  :  output += "§"; break;
        case "&uml;"   :  output += "¨"; break;
        case "&copy;"  :  output += "©"; break;
        case "&ordf;"  :  output += "ª"; break;
        case "&laquo;" :  output += "«"; break;
        case "&not;"   :  output += "¬"; break;
        case "&shy;"   :  output += "­"; break;
        case "&reg;"   :  output += "®"; break;
        case "&macr;"  :  output += "¯"; break;
        case "&deg;"   :  output += "°"; break;
        case "&plusmn;":  output += "±"; break;
        case "&sup2;"  :  output += "²"; break;
        case "&sup3;"  :  output += "³"; break;
        case "&acute;" :  output += "´"; break;
        case "&micro;" :  output += "µ"; break;
        case "&para;"  :  output += "¶"; break;
        case "&middot;":  output += "·"; break;
        case "&cedil;" :  output += "¸"; break;
        case "&sup1;"  :  output += "¹"; break;
        case "&ordm;"  :  output += "º"; break;
        case "&raquo;" :  output += "»"; break;
        case "&frac14;":  output += "¼"; break;
        case "&frac12;":  output += "½"; break;
        case "&frac34;":  output += "¾"; break;
        case "&iquest;":  output += "¿"; break;
        case "&Agrave;":  output += "À"; break;
        case "&Aacute;":  output += "Á"; break;
        case "&Acirc;" :  output += "Â"; break;
        case "&Atilde;":  output += "Ã"; break;
        case "&Auml;"  :  output += "Ä"; break;
        case "&Aring;" :  output += "Å"; break;
        case "&AElig;" :  output += "Æ"; break;
        case "&Ccedil;":  output += "Ç"; break;
        case "&Egrave;":  output += "È"; break;
        case "&Eacute;":  output += "É"; break;
        case "&Ecirc;" :  output += "Ê"; break;
        case "&Euml;"  :  output += "Ë"; break;
        case "&Igrave;":  output += "Ì"; break;
        case "&Iacute;":  output += "Í"; break;
        case "&Icirc;" :  output += "Î"; break;
        case "&Iuml;"  :  output += "Ï"; break;
        case "&ETH;"   :  output += "Ð"; break;
        case "&Ntilde;":  output += "Ñ"; break;
        case "&Ograve;":  output += "Ò"; break;
        case "&Oacute;":  output += "Ó"; break;
        case "&Ocirc;" :  output += "Ô"; break;
        case "&Otilde;":  output += "Õ"; break;
        case "&Ouml;"  :  output += "Ö"; break;
        case "&times;" :  output += "×"; break;
        case "&Oslash;":  output += "Ø"; break;
        case "&Ugrave;":  output += "Ù"; break;
        case "&Uacute;":  output += "Ú"; break;
        case "&Ucirc;" :  output += "Û"; break;
        case "&Uuml;"  :  output += "Ü"; break;
        case "&Yacute;":  output += "Ý"; break;
        case "&THORN;" :  output += "Þ"; break;
        case "&szlig;" :  output += "ß"; break;
        case "&agrave;":  output += "à"; break;
        case "&aacute;":  output += "á"; break;
        case "&acirc;" :  output += "â"; break;
        case "&atilde;":  output += "ã"; break;
        case "&auml;"  :  output += "ä"; break;
        case "&aring;" :  output += "å"; break;
        case "&aelig;" :  output += "æ"; break;
        case "&ccedil;":  output += "ç"; break;
        case "&egrave;":  output += "è"; break;
        case "&eacute;":  output += "é"; break;
        case "&ecirc;" :  output += "ê"; break;
        case "&euml;"  :  output += "ë"; break;
        case "&igrave;":  output += "ì"; break;
        case "&iacute;":  output += "í"; break;
        case "&icirc;" :  output += "î"; break;
        case "&iuml;"  :  output += "ï"; break;
        case "&eth;"   :  output += "ð"; break;
        case "&ntilde;":  output += "ñ"; break;
        case "&ograve;":  output += "ò"; break;
        case "&oacute;":  output += "ó"; break;
        case "&ocirc;" :  output += "ô"; break;
        case "&otilde;":  output += "õ"; break;
        case "&ouml;"  :  output += "ö"; break;
        case "&divide;":  output += "÷"; break;
        case "&oslash;":  output += "ø"; break;
        case "&ugrave;":  output += "ù"; break;
        case "&uacute;":  output += "ú"; break;
        case "&ucirc;" :  output += "û"; break;
        case "&uuml;"  :  output += "ü"; break;
        case "&yacute;":  output += "ý"; break;
        case "&thorn;" :  output += "þ"; break;
        case "&yuml;"  :  output += "ÿ"; break;
        case "&hellip;"  :  output += "…"; break;
        case "&ldquo;"  :  output += "“"; break;
        case "&rdquo;"  :  output += "”"; break;
        default        :
          output += sscanf(bit, "&#%+~U;") ? sprintf("%c", to_int(bit[2..<2])) : bit;
      }
    else
      output += bit;
  }

  return output;
}

