/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package assignment3;

/**
 *
 * @author mille
 */
public class Country {

    public String Code;
    public String Name;
    public String Description;
    
    public enum Continents {
        ASIA, SOUTH_AMERICA, EUROPE, AFRICA, AUSTRALIA, NORTH_AMERICA
    }
    public Continents Continent;
    
    public Country(String code, String name, String continent) {
        Code = code;
        Name = toTitleCase(name);
        Continent = parseContinent(continent);
        Description = formDescription(name);
    }

    public static String formDescription(String name) {
        int words = name.split(" ").length;
        Boolean containsV = name.indexOf('V') >= 0;
        
        String retVal = "This country contains ";
        if (words == 1)
            retVal += "only ";
        retVal += words;
        retVal += " word";
        if (words > 1)
            retVal += "s";
        retVal += " and does ";
        if (!containsV)
            retVal += "not ";
        retVal += "contain the letter 'v'";
        return retVal;
    }
    
    public static String toTitleCase(String givenString) {
        String[] arr = givenString.split(" ");
        StringBuilder sb = new StringBuilder();

        for (String arr1 : arr) {
            sb.append(Character.toUpperCase(arr1.charAt(0))).append(arr1.substring(1)).append(" ");
        }
        return sb.toString().trim();
    }

    private static Continents parseContinent(String c) {
        switch (c) {
            case "AFRICA":
                return Continents.AFRICA;
            case "ASIA":
                return Continents.ASIA;
            case "AUSTRALIA":
                return Continents.AUSTRALIA;
            case "EUROPE":
                return Continents.EUROPE;
            case "NORTH AMERICA":
                return Continents.NORTH_AMERICA;
            case "SOUTH AMERICA":
                return Continents.SOUTH_AMERICA;
            default:
                return Continents.NORTH_AMERICA;
        }
    }

    public static Country getCountry(String name) {
        for (Country country: Country.getCountries()) {
            if (country.Name.equals(name)) {
                return country;
            }
        }

        return null;
    }

    public static Country[] getCountries() {
        Country[] countries = new Country[202];
        countries[1] = new Country("AO", "ANGOLA", "AFRICA");
        countries[2] = new Country("BF", "BURKINA FASO", "AFRICA");
        countries[3] = new Country("BI", "BURUNDI", "AFRICA");
        countries[4] = new Country("BJ", "BENIN", "AFRICA");
        countries[5] = new Country("BW", "BOTSWANA", "AFRICA");
        countries[6] = new Country("CD", "DEMOCRATIC REPUBLIC OF CONGO", "AFRICA");
        countries[7] = new Country("CF", "CENTRAL AFRICAN REPUBLIC", "AFRICA");
        countries[8] = new Country("CG", "CONGO", "AFRICA");
        countries[9] = new Country("CI", "IVORY COAST", "AFRICA");
        countries[10] = new Country("CM", "CAMEROON", "AFRICA");
        countries[11] = new Country("CV", "CAPE VERDE", "AFRICA");
        countries[12] = new Country("DJ", "DJIBOUTI", "AFRICA");
        countries[13] = new Country("DZ", "ALGERIA", "AFRICA");
        countries[14] = new Country("EG", "EGYPT", "AFRICA");
        countries[15] = new Country("ER", "ERITREA", "AFRICA");
        countries[16] = new Country("ES", "CANARY ISLANDS (SPAIN)", "AFRICA");
        countries[17] = new Country("ET", "ETHIOPIA", "AFRICA");
        countries[18] = new Country("GA", "GABON", "AFRICA");
        countries[19] = new Country("GH", "GHANA", "AFRICA");
        countries[20] = new Country("GM", "GAMBIA", "AFRICA");
        countries[21] = new Country("GN", "GUINEA, REPUBLIC OF", "AFRICA");
        countries[22] = new Country("GQ", "GUINEA EQUATORIAL", "AFRICA");
        countries[23] = new Country("GW", "GUINEA BISSAU", "AFRICA");
        countries[24] = new Country("IO", "BRITISH INDIAN OCEAN TERRITOry(", "AFRICA");
        countries[25] = new Country("KE", "KENYA", "AFRICA");
        countries[26] = new Country("LR", "LIBERIA", "AFRICA");
        countries[27] = new Country("LS", "LESOTHO", "AFRICA");
        countries[28] = new Country("LY", "LIBYAN ARAB JAMAHIRIYA", "AFRICA");
        countries[29] = new Country("MA", "MOROCCO", "AFRICA");
        countries[30] = new Country("MG", "MADAGASCAR/COMOROS", "AFRICA");
        countries[31] = new Country("ML", "MALI", "AFRICA");
        countries[32] = new Country("MR", "MAURITANIA", "AFRICA");
        countries[33] = new Country("MU", "MAURITIUS", "AFRICA");
        countries[34] = new Country("MW", "MALAWI", "AFRICA");
        countries[35] = new Country("MZ", "MOZAMBIQUE", "AFRICA");
        countries[36] = new Country("NA", "NAMIBIA", "AFRICA");
        countries[37] = new Country("NE", "NIGER", "AFRICA");
        countries[38] = new Country("NG", "NIGERIA", "AFRICA");
        countries[39] = new Country("RW", "RWANDA", "AFRICA");
        countries[40] = new Country("SC", "SEYCHELLES", "AFRICA");
        countries[41] = new Country("SD", "SUDAN", "AFRICA");
        countries[42] = new Country("SL", "SIERRA LEONE", "AFRICA");
        countries[43] = new Country("SN", "SENEGAL", "AFRICA");
        countries[44] = new Country("SO", "SOMALIA", "AFRICA");
        countries[45] = new Country("ST", "SAO TOME AND PRINCIPE", "AFRICA");
        countries[46] = new Country("SZ", "SWAZILAND", "AFRICA");
        countries[47] = new Country("TD", "CHAD", "AFRICA");
        countries[48] = new Country("TG", "TOGO", "AFRICA");
        countries[49] = new Country("TM", "TUNISIA", "AFRICA");
        countries[50] = new Country("TZ", "TANZANIA, UNITED REPUBLIC OF", "AFRICA");
        countries[51] = new Country("UG", "UGANDA", "AFRICA");
        countries[52] = new Country("ZA", "SOUTH AFRICA", "AFRICA");
        countries[53] = new Country("ZM", "ZAMBIA", "AFRICA");
        countries[54] = new Country("ZW", "ZIMBABWE", "AFRICA");
        countries[55] = new Country("AE", "UNITED ARAB EMIRATES", "ASIA");
        countries[56] = new Country("AF", "AFGHANISTAN", "ASIA");
        countries[57] = new Country("AM", "ARMENIA", "ASIA");
        countries[58] = new Country("AZ", "AZERBAIJAN", "ASIA");
        countries[59] = new Country("BD", "BANGLADESH", "ASIA");
        countries[60] = new Country("BH", "BAHRAIN", "ASIA");
        countries[61] = new Country("BN", "BRUNEI DARUSSALAM", "ASIA");
        countries[62] = new Country("BT", "BHUTAN", "ASIA");
        countries[63] = new Country("CN", "CHINA", "ASIA");
        countries[64] = new Country("GE", "GEORGIA", "ASIA");
        countries[65] = new Country("HK", "HONG KONG", "ASIA");
        countries[66] = new Country("ID", "INDONESIA", "ASIA");
        countries[67] = new Country("IL", "ISRAEL", "ASIA");
        countries[68] = new Country("IN", "INDIA", "ASIA");
        countries[69] = new Country("IQ", "IRAQ", "ASIA");
        countries[70] = new Country("IR", "IRAN, ISLAMIC REPUBLIC", "ASIA");
        countries[71] = new Country("JO", "JORDAN", "ASIA");
        countries[72] = new Country("JP", "JAPAN", "ASIA");
        countries[73] = new Country("KH", "CAMBODIA", "ASIA");
        countries[74] = new Country("KP", "KOREA, DEMOCRATIC PEOPLES REPUBLIC", "ASIA");
        countries[75] = new Country("KR", "KOREA, REPUBLIC OF", "ASIA");
        countries[76] = new Country("KW", "KUWAIT", "ASIA");
        countries[77] = new Country("KZ", "KAZAKHSTAN", "ASIA");
        countries[78] = new Country("LA", "LAOS PEOPLES DEMOCRATIC REPUBLIC", "ASIA");
        countries[79] = new Country("LK", "SRI LANKA", "ASIA");
        countries[80] = new Country("LP", "LEBANON", "ASIA");
        countries[81] = new Country("MM", "MYANMAR", "ASIA");
        countries[82] = new Country("MN", "MONGOLIA", "ASIA");
        countries[83] = new Country("MO", "MACAU", "ASIA");
        countries[84] = new Country("MV", "MALDIVES", "ASIA");
        countries[85] = new Country("MY", "MALAYSIA", "ASIA");
        countries[86] = new Country("NP", "NEPAL", "ASIA");
        countries[87] = new Country("OM", "OMAN", "ASIA");
        countries[88] = new Country("PH", "PHILIPPINES", "ASIA");
        countries[89] = new Country("PK", "PAKISTAN", "ASIA");
        countries[90] = new Country("QA", "QATAR", "ASIA");
        countries[91] = new Country("RU", "CIS/RUSSIAN FEDERATION", "ASIA");
        countries[92] = new Country("SA", "SAUDI ARABIA", "ASIA");
        countries[93] = new Country("SG", "SINGAPORE", "ASIA");
        countries[94] = new Country("SY", "SYRIAN ARAB REPUBLIC", "ASIA");
        countries[95] = new Country("TH", "THAILAND", "ASIA");
        countries[96] = new Country("TL", "EAST TIMOR", "ASIA");
        countries[97] = new Country("TW", "TAIWAN", "ASIA");
        countries[98] = new Country("UZ", "UZBEKISTAN", "ASIA");
        countries[99] = new Country("VN", "VIETNAM", "ASIA");
        countries[100] = new Country("YE", "YEMEN REPUBLIC", "ASIA");
        countries[101] = new Country("AU", "AUSTRALIA", "AUSTRALIA");
        countries[102] = new Country("CK", "COOK ISLANDS", "AUSTRALIA");
        countries[103] = new Country("FJ", "FIJI", "AUSTRALIA");
        countries[104] = new Country("KI", "KIRIBATI AND TUVALU", "AUSTRALIA");
        countries[105] = new Country("NC", "NEW CALEDONIA", "AUSTRALIA");
        countries[106] = new Country("NU", "NIUE ISLANDS", "AUSTRALIA");
        countries[107] = new Country("PF", "FRENCH POLYNESIA", "AUSTRALIA");
        countries[108] = new Country("VU", "VANUATU", "AUSTRALIA");
        countries[109] = new Country("WF", "WALLIS ISLANDS AND FUTUNA", "AUSTRALIA");
        countries[110] = new Country("WS", "SAMOA/AMERICAN SAMOA", "AUSTRALIA");
        countries[111] = new Country("FM", "MICRONESIA", "AUSTRALIA");
        countries[112] = new Country("MH", "MARSHALL ISLANDS", "AUSTRALIA");
        countries[113] = new Country("NR", "NAURU", "AUSTRALIA");
        countries[114] = new Country("NZ", "NEW ZEALAND", "AUSTRALIA");
        countries[115] = new Country("PG", "PAPUA NEW GUINEA", "AUSTRALIA");
        countries[116] = new Country("SB", "SOLOMON ISLANDS", "AUSTRALIA");
        countries[117] = new Country("UM", "JOHNSTON ISLANDS (US)EXTERNAL", "AUSTRALIA");
        countries[118] = new Country("AL", "ALBANIA", "EUROPE");
        countries[119] = new Country("AT", "AUSTRIA", "EUROPE");
        countries[120] = new Country("BA", "BOSNIA AND HERZEGOVINA", "EUROPE");
        countries[121] = new Country("BE", "BELGIUM", "EUROPE");
        countries[122] = new Country("BG", "BULGARIA", "EUROPE");
        countries[123] = new Country("BY", "BELARUS", "EUROPE");
        countries[124] = new Country("CH", "SWITZERLAND", "EUROPE");
        countries[125] = new Country("CY", "CYPRUS", "EUROPE");
        countries[126] = new Country("CZ", "CZECH REPUBLIC", "EUROPE");
        countries[127] = new Country("DE", "GERMANY, FEDERAL REPUBLIC OF", "EUROPE");
        countries[128] = new Country("DK", "DENMARK", "EUROPE");
        countries[129] = new Country("EE", "ESTONIA", "EUROPE");
        countries[130] = new Country("FI", "FINLAND", "EUROPE");
        countries[131] = new Country("FR", "FRANCE", "EUROPE");
        countries[132] = new Country("GB", "UNITED KINGDOM", "EUROPE");
        countries[133] = new Country("GR", "GREECE", "EUROPE");
        countries[134] = new Country("HR", "CROATIA", "EUROPE");
        countries[135] = new Country("HU", "HUNGAry(", "EUROPE");
        countries[136] = new Country("IE", "IRELAND", "EUROPE");
        countries[137] = new Country("IS", "ICELAND", "EUROPE");
        countries[138] = new Country("IT", "ITALY", "EUROPE");
        countries[139] = new Country("LT", "LITHUANIA", "EUROPE");
        countries[140] = new Country("LU", "LUXEMBOURG", "EUROPE");
        countries[141] = new Country("LV", "LATVIA", "EUROPE");
        countries[142] = new Country("MD", "MOLDOVA", "EUROPE");
        countries[143] = new Country("MK", "REPUBLIC OF MACEDONIA", "EUROPE");
        countries[144] = new Country("MT", "MALTA", "EUROPE");
        countries[145] = new Country("NL", "NETHERLANDS", "EUROPE");
        countries[146] = new Country("NO", "NORWAY", "EUROPE");
        countries[147] = new Country("PL", "POLAND", "EUROPE");
        countries[148] = new Country("PT", "PORTUGAL, MADEIRA AND AZORES", "EUROPE");
        countries[149] = new Country("RO", "ROMANIA", "EUROPE");
        countries[150] = new Country("RS", "KOSOVO", "EUROPE");
        countries[151] = new Country("SE", "SWEDEN", "EUROPE");
        countries[152] = new Country("SI", "SLOVENIA", "EUROPE");
        countries[153] = new Country("SK", "SLOVAKIAN REPUBLIC", "EUROPE");
        countries[154] = new Country("TR", "TURKEY", "EUROPE");
        countries[155] = new Country("UA", "UKRAINE", "EUROPE");
        countries[156] = new Country("GL", "GREENLAND (DENMARK)", "NORTH AMERICA");
        countries[157] = new Country("CA", "CANADA", "NORTH AMERICA");
        countries[158] = new Country("US", "UNITED STATES", "NORTH AMERICA");
        countries[159] = new Country("AG", "ANTIGUA AND BARBUDA", "SOUTH AMERICA");
        countries[160] = new Country("AI", "ANGUILLA ISLANDS", "SOUTH AMERICA");
        countries[161] = new Country("AN", "NETHERLANDS ANTILLES", "SOUTH AMERICA");
        countries[162] = new Country("AR", "ARGENTINA", "SOUTH AMERICA");
        countries[163] = new Country("BB", "BARBADOS", "SOUTH AMERICA");
        countries[164] = new Country("BM", "BERMUDA", "SOUTH AMERICA");
        countries[165] = new Country("BO", "BOLIVIA", "SOUTH AMERICA");
        countries[166] = new Country("BR", "BRASIL", "SOUTH AMERICA");
        countries[167] = new Country("BS", "BAHAMAS", "SOUTH AMERICA");
        countries[168] = new Country("BZ", "BELIZE", "SOUTH AMERICA");
        countries[169] = new Country("CL", "CHILE, EASTER ISLANDS", "SOUTH AMERICA");
        countries[170] = new Country("CO", "COLOMBIA", "SOUTH AMERICA");
        countries[171] = new Country("CR", "COSTA RICA", "SOUTH AMERICA");
        countries[172] = new Country("CU", "CUBA", "SOUTH AMERICA");
        countries[173] = new Country("DM", "DOMINICA", "SOUTH AMERICA");
        countries[174] = new Country("DO", "DOMINICAN REPUBLIC", "SOUTH AMERICA");
        countries[175] = new Country("EC", "ECUADOR", "SOUTH AMERICA");
        countries[176] = new Country("FK", "FALKLAND ISLANDS", "SOUTH AMERICA");
        countries[177] = new Country("GD", "GRENADA", "SOUTH AMERICA");
        countries[178] = new Country("GF", "FRENCH GUYANA", "SOUTH AMERICA");
        countries[179] = new Country("GT", "GUATEMALA", "SOUTH AMERICA");
        countries[180] = new Country("GY", "GUYANA", "SOUTH AMERICA");
        countries[181] = new Country("HN", "HONDURAS", "SOUTH AMERICA");
        countries[182] = new Country("HT", "HAITI", "SOUTH AMERICA");
        countries[183] = new Country("JM", "JAMAICA, GRANADA AND SANTA LUCIA", "SOUTH AMERICA");
        countries[184] = new Country("KN", "ST. KITTS AND NEVIS", "SOUTH AMERICA");
        countries[185] = new Country("KY", "CAYMAN ISLANDS", "SOUTH AMERICA");
        countries[186] = new Country("LC", "SAINT LUCIA", "SOUTH AMERICA");
        countries[187] = new Country("MS", "MONTSERRAT ISLANDS", "SOUTH AMERICA");
        countries[188] = new Country("MX", "MEXICO", "SOUTH AMERICA");
        countries[189] = new Country("NI", "NICARAGUA", "SOUTH AMERICA");
        countries[190] = new Country("PA", "PANAMA", "SOUTH AMERICA");
        countries[191] = new Country("PE", "PERU", "SOUTH AMERICA");
        countries[192] = new Country("PR", "UNITED STATES / PUERTO RICO", "SOUTH AMERICA");
        countries[193] = new Country("PY", "PARAGUAY", "SOUTH AMERICA");
        countries[194] = new Country("SR", "SURINAME", "SOUTH AMERICA");
        countries[195] = new Country("SV", "EL SALVADOR", "SOUTH AMERICA");
        countries[196] = new Country("TC", "TURKS AND CAICOS ISLANDS", "SOUTH AMERICA");
        countries[197] = new Country("TT", "TRINIDAD AND TOBAGO", "SOUTH AMERICA");
        countries[198] = new Country("UY", "URUGUAY", "SOUTH AMERICA");
        countries[199] = new Country("VC", "ST. VINCENT AND GRENADINES", "SOUTH AMERICA");
        countries[200] = new Country("VE", "VENEZUELA", "SOUTH AMERICA");
        countries[201] = new Country("VG", "VIRGIN ISLANDS (UK)", "SOUTH AMERICA");
        countries[0] = new Country("VI", "VIRGIN ISLANDS (US)", "SOUTH AMERICA");
        return countries;
    }
}
