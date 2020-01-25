#pragma once
#include "itl.h"

String make_header(const String& title) {
	String s = FPSTR(WEB_PAGE_HEADER);
	s.replace("{tt}", FPSTR(INTL_FEINSTAUBSENSOR));
	s.replace("{h}", FPSTR(INTL_UBERSICHT));
	if(title != " ") {
		s.replace("{n}", F("&raquo;"));
	} else {
		s.replace("{n}", "");
	}
	s.replace("{t}", title);
	s.replace("{id}", esp_chipid);
	s.replace("{mac}", WiFi.macAddress());
	s.replace("{fwt}", FPSTR(INTL_FIRMWARE));
	s.replace("{fw}", SOFTWARE_VERSION);
	return s;
}

String make_footer() {
	String s = FPSTR(WEB_PAGE_FOOTER);
	s.replace("{t}", FPSTR(INTL_ZURUCK_ZUR_STARTSEITE));
	return s;
}

String form_input(const String& name, const String& info, const String& value, const int length) {
	String s = F("<tr><td>{i} </td><td style='width:65%;'><input type='text' name='{n}' id='{n}' placeholder='{i}' value='{v}' maxlength='{l}'/></td></tr>");
	String t_value = value;
	t_value.replace("'", "&#39;");
	s.replace("{i}", info);
	s.replace("{n}", name);
	s.replace("{v}", t_value);
	s.replace("{l}", String(length));
	return s;
}

String form_password(const String& name, const String& info, const String& value, const int length) {
	String s = F("<tr><td>{i} </td><td style='width:65%;'><input type='password' name='{n}' id='{n}' placeholder='{i}' value='{v}' maxlength='{l}'/></td></tr>");
	String password = "";
	for (uint8_t i = 0; i < value.length(); i++) {
		password += "*";
	}
	s.replace("{i}", info);
	s.replace("{n}", name);
	s.replace("{v}", password);
	s.replace("{l}", String(length));
	return s;
}

String form_checkbox(const String& name, const String& info, const bool checked, const bool linebreak = true) {
	String s = F("<tr><td>{i} </td><td style='width:65%;'><label for='{n}'><input type='checkbox' name='{n}' value='1' id='{n}' {c}/> {i}</label></td></tr>");
	if (checked) {
		s.replace("{c}", F(" checked='checked'"));
	} else {
		s.replace("{c}", "");
	};
	s.replace("{i}", info);
	s.replace("{n}", name);
	if (! linebreak) {
		s.replace("<br/>", "");
	}
	return s;
}

String form_submit(const String& value) {
	String s = F("<tr><td>&nbsp;</td><td><input type='submit' name='submit' value='{v}' /></td></tr>");
	s.replace("{v}", value);
	return s;
}
