#include "unity.h"
#include "LI8x0.h"
#include "buffer_c.h"
#include "sysinfo.h"
#include "mock_bsp.h"
#include "mock_errors.h"
#include "mock_AM08x5.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_LI8x0_Init_should_InitializeUart_and_Setup(void){

    BSP_UART_Init_ExpectAndReturn(&pLicor1->UART,BSP_OK);
    BSP_GPIO_Init_Expect(&pLicor1->Power);

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_Init());
}


void test_LI8x0_FactoryReset_should_reset_LicorStructure(void){

    sLicor_t L1;
    sLicor_t *pL1 = &L1;

    LI8x0_FactoryReset(pL1);

    TEST_ASSERT_EQUAL(LICOR_CO2_OUTPUT,L1.sysconfig.rs232.co2);
    TEST_ASSERT_EQUAL(LICOR_CELLPRES_OUTPUT,L1.sysconfig.rs232.cellpres);
    TEST_ASSERT_EQUAL(LICOR_ECHO_OUTPUT,L1.sysconfig.rs232.echo);
}



void test_LIx80_Level1Str_should_CreateValidStr_and_ReturnValid(void){
  
    eLicorXMLLvl1_t LVL1 = LICOR_XML_LVL1_820;

    char sstr[32];
    char *psstr = &sstr[0];
    char estr[32];
    char *pestr = &estr[0];

    uint16_t slen;
    uint16_t elen;

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_Level1Str(LVL1,psstr,&slen,pestr,&elen));

    TEST_ASSERT_EQUAL_STRING("<LI820>",sstr);
    TEST_ASSERT_EQUAL_STRING("</LI820>",estr);
    TEST_ASSERT_EQUAL(7,slen);
    TEST_ASSERT_EQUAL(8,elen);

}

void test_LIx80_Level1Str_should_ProduceError_and_ReturnInvalid_for_InvalidType(void){

    eLicorXMLLvl1_t LVL = 0x11;

    char sstr[32];
    char *psstr = &sstr[0];
    char estr[32];
    char *pestr = &estr[0];

    memset(sstr,0,32);
    memset(estr,0,32);

    uint16_t slen=0;
    uint16_t elen=0;

    Error_Expect(ERROR_MSG_7100,LOG_Priority_Med);
    TEST_ASSERT_EQUAL(LICOR_FAIL,LI8x0_Level1Str(LVL,psstr,&slen,pestr,&elen));

    TEST_ASSERT_EQUAL_STRING("",sstr);
    TEST_ASSERT_EQUAL_STRING("",estr);
    TEST_ASSERT_EQUAL(0,slen);
    TEST_ASSERT_EQUAL(0,elen);

}


void test_LIx80_Lvel2Str_should_CreateValidStr_and_ReturnValid(void){
    eLicorXMLLvl2_t LVL2 = LICOR_XML_LVL2_AUXDATA;
    char sstr[32];
    char *psstr = &sstr[0];
    char estr[32];
    char *pestr = &estr[0];
    memset(sstr,0,32);
    memset(estr,0,32);

    uint16_t slen = 0;
    uint16_t elen = 0;

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_Level2Str(LVL2,psstr,&slen,pestr,&elen));

    TEST_ASSERT_EQUAL_STRING("<AUXDATA>",sstr);
    TEST_ASSERT_EQUAL_STRING("</AUXDATA>",estr);
    TEST_ASSERT_EQUAL(9,slen);
    TEST_ASSERT_EQUAL(10,elen);
    
}

void test_LIx80_Level2Str_should_ProduceError_and_ReturnInvalid_for_InvalidType(void){

    eLicorXMLLvl2_t LVL = 0xF1;

    char sstr[32];
    char *psstr = &sstr[0];
    char estr[32];
    char *pestr = &estr[0];

    memset(sstr,0,32);
    memset(estr,0,32);

    uint16_t slen=0;
    uint16_t elen=0;

    Error_Expect(ERROR_MSG_7101,LOG_Priority_Med);
    TEST_ASSERT_EQUAL(LICOR_FAIL,LI8x0_Level2Str(LVL,psstr,&slen,pestr,&elen));

    TEST_ASSERT_EQUAL_STRING("",sstr);
    TEST_ASSERT_EQUAL_STRING("",estr);
    TEST_ASSERT_EQUAL(0,slen);
    TEST_ASSERT_EQUAL(0,elen);

}

void test_LIx80_Lvel3Str_should_CreateValidStr_and_ReturnValid(void){
    eLicorXMLLvl3_t LVL3 = LICOR_XML_LVL3_DACS;
    char sstr[32];
    char *psstr = &sstr[0];
    char estr[32];
    char *pestr = &estr[0];
    memset(sstr,0,32);
    memset(estr,0,32);

    uint16_t slen = 0;
    uint16_t elen = 0;

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_Level3Str(LVL3,psstr,&slen,pestr,&elen));

    TEST_ASSERT_EQUAL_STRING("<DACS>",sstr);
    TEST_ASSERT_EQUAL_STRING("</DACS>",estr);
    TEST_ASSERT_EQUAL(6,slen);
    TEST_ASSERT_EQUAL(7,elen);
    
}

void test_LIx80_Level3Str_should_ProduceError_and_ReturnInvalid_for_InvalidType(void){

    eLicorXMLLvl3_t LVL = 0xFF;

    char sstr[32];
    char *psstr = &sstr[0];
    char estr[32];
    char *pestr = &estr[0];

    memset(sstr,0,32);
    memset(estr,0,32);

    uint16_t slen=0;
    uint16_t elen=0;

    Error_Expect(ERROR_MSG_7102,LOG_Priority_Med);
    TEST_ASSERT_EQUAL(LICOR_FAIL,LI8x0_Level3Str(LVL,psstr,&slen,pestr,&elen));

    TEST_ASSERT_EQUAL_STRING("",sstr);
    TEST_ASSERT_EQUAL_STRING("",estr);
    TEST_ASSERT_EQUAL(0,slen);
    TEST_ASSERT_EQUAL(0,elen);

}

void test_LI8x0_Level4Str_should_CreateValidStr_and_ReturnValid(void){
    eLicorXMLLvl3_t LVL = LICOR_XML_LVL4_OFFSET;
    char sstr[32];
    char *psstr = &sstr[0];
    char estr[32];
    char *pestr = &estr[0];
    memset(sstr,0,32);
    memset(estr,0,32);

    uint16_t slen = 0;
    uint16_t elen = 0;

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_Level4Str(LVL,psstr,&slen,pestr,&elen));

    TEST_ASSERT_EQUAL_STRING("<OFFSET>",sstr);
    TEST_ASSERT_EQUAL_STRING("</OFFSET>",estr);
    TEST_ASSERT_EQUAL(8,slen);
    TEST_ASSERT_EQUAL(9,elen);
}

void test_LIx80_Level4Str_should_ProduceError_and_ReturnInvalid_for_InvalidType(void){

    eLicorXMLLvl4_t LVL = 0x6F;

    char sstr[32];
    char *psstr = &sstr[0];
    char estr[32];
    char *pestr = &estr[0];

    memset(sstr,0,32);
    memset(estr,0,32);

    uint16_t slen=0;
    uint16_t elen=0;

    Error_Expect(ERROR_MSG_7103,LOG_Priority_Med);
    TEST_ASSERT_EQUAL(LICOR_FAIL,LI8x0_Level4Str(LVL,psstr,&slen,pestr,&elen));

    TEST_ASSERT_EQUAL_STRING("",sstr);
    TEST_ASSERT_EQUAL_STRING("",estr);
    TEST_ASSERT_EQUAL(0,slen);
    TEST_ASSERT_EQUAL(0,elen);

}






void test_LI8x0_ValidateXML_should_ReturnValid_for_ValidSetofNone(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_NONE,
                        .LVL2 = LICOR_XML_LVL2_NONE,
                        .LVL3 = LICOR_XML_LVL3_NONE,
                        .LVL4 = LICOR_XML_LVL4_NONE };


    
    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ValidateXML(&XML));

}

void test_LI8x0_ValidateXML_should_ReturnInvalid_AndError_for_InvalidSetofNone(void){


    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_NONE,
                        .LVL2 = LICOR_XML_LVL2_DATA,
                        .LVL3 = LICOR_XML_LVL3_NONE,
                        .LVL4 = LICOR_XML_LVL4_NONE };


    Error_Expect(ERROR_MSG_7110,LOG_Priority_Med);
    TEST_ASSERT_EQUAL(LICOR_FAIL,LI8x0_ValidateXML(&XML));

}


void test_LI8x0_ValidateXML_should_ReturnInvalid_AndError_for_H2O_in_LI820(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_820,
                        .LVL2 = LICOR_XML_LVL2_CAL,
                        .LVL3 = LICOR_XML_LVL3_H2OSPAN,
                        .LVL4 = LICOR_XML_LVL4_NONE };

    Error_Expect(ERROR_MSG_7121, LOG_Priority_Med);
    TEST_ASSERT_EQUAL(LICOR_FAIL,LI8x0_ValidateXML(&XML));
}

void test_LI8x0_ValidXML_Level3_should_ReturnValid_for_Valid_Source(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_830,
                        .LVL2 = LICOR_XML_LVL2_TECH,
                        .LVL3 = LICOR_XML_LVL3_SOURCE,
                        .LVL4 = LICOR_XML_LVL4_VOLTAGE };

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ValidXML_Level3(&XML));

}

void test_LI8x0_ValidXML_Level3_should_ReturnInvalid_for_InvalidSource(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_830,
                        .LVL2 = LICOR_XML_LVL2_TECH,
                        .LVL3 = LICOR_XML_LVL3_SOURCE,
                        .LVL4 = LICOR_XML_LVL4_CO2 };

    Error_Expect(ERROR_MSG_7114,LOG_Priority_Med);
    TEST_ASSERT_EQUAL(LICOR_FAIL,LI8x0_ValidXML_Level3(&XML));

}


void test_LI8x0_ValidXML_Level3_should_ReturnValid_for_Valid_Pump(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_830,
                        .LVL2 = LICOR_XML_LVL2_TECH,
                        .LVL3 = LICOR_XML_LVL3_PUMP,
                        .LVL4 = LICOR_XML_LVL4_MODEL };

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ValidXML_Level3(&XML));

}

void test_LI8x0_ValidXML_Level3_should_ReturnInvalid_for_Invalid_Pump(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_830,
                        .LVL2 = LICOR_XML_LVL2_TECH,
                        .LVL3 = LICOR_XML_LVL3_PUMP,
                        .LVL4 = LICOR_XML_LVL4_CO2 };

    Error_Expect(ERROR_MSG_7114,LOG_Priority_Med);
    TEST_ASSERT_EQUAL(LICOR_FAIL,LI8x0_ValidXML_Level3(&XML));

}



void test_LI8x0_ValidXML_Level3_should_returnValid_for_Valid_CO2(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_830,
                        .LVL2 = LICOR_XML_LVL2_RS232,
                        .LVL3 = LICOR_XML_LVL3_CO2,
                        .LVL4 = LICOR_XML_LVL4_NONE };

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ValidXML_Level3(&XML));

    XML.LVL2 = LICOR_XML_LVL2_POLY;
    XML.LVL4 = LICOR_XML_LVL4_A1;

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ValidXML_Level3(&XML));
}


void test_LI8x0_ValidXML_Level3_should_returnInValid_for_InValid_CO2(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_830,
                        .LVL2 = LICOR_XML_LVL2_RS232,
                        .LVL3 = LICOR_XML_LVL3_CO2,
                        .LVL4 = LICOR_XML_LVL4_A1 };

    Error_Expect(ERROR_MSG_7114,LOG_Priority_Med);
    TEST_ASSERT_EQUAL(LICOR_FAIL,LI8x0_ValidXML_Level3(&XML));


}


void test_LI8x0_ValidXML_Level3_should_returnValid_for_Valid_H2O(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_830,
                        .LVL2 = LICOR_XML_LVL2_RS232,
                        .LVL3 = LICOR_XML_LVL3_H2O,
                        .LVL4 = LICOR_XML_LVL4_NONE };

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ValidXML_Level3(&XML));

    XML.LVL2 = LICOR_XML_LVL2_POLY;
    XML.LVL4 = LICOR_XML_LVL4_A2;

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ValidXML_Level3(&XML));
}


void test_LI8x0_ValidXML_Level3_should_returnInValid_for_InValid_H2O(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_830,
                        .LVL2 = LICOR_XML_LVL2_RS232,
                        .LVL3 = LICOR_XML_LVL3_H2O,
                        .LVL4 = LICOR_XML_LVL4_A3 };

    Error_Expect(ERROR_MSG_7114,LOG_Priority_Med);
    TEST_ASSERT_EQUAL(LICOR_FAIL,LI8x0_ValidXML_Level3(&XML));


}

void test_LI8x0_ValidXML_Level2_should_ReturnValid_for_ValidSetoAuxdata(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_820,
                        .LVL2 = LICOR_XML_LVL2_AUXDATA,
                        .LVL3 = LICOR_XML_LVL3_NONE,
                        .LVL4 = LICOR_XML_LVL4_NONE };


    
    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ValidXML_Level2(&XML));

}



void test_LI8x0_Validate_NoPump_should_ReturnValid_for_NoPumpCommand(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_820,
                        .LVL2 = LICOR_XML_LVL2_AUXDATA,
                        .LVL3 = LICOR_XML_LVL3_NONE,
                        .LVL4 = LICOR_XML_LVL4_NONE };


    
    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_Validate_NoPump(&XML));
    

}

void test_LI8x0_Validate_NoPump_should_ReturnInvalid_for_PumpCommand(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_820,
                        .LVL2 = LICOR_XML_LVL2_AUXDATA,
                        .LVL3 = LICOR_XML_LVL3_PUMP,
                        .LVL4 = LICOR_XML_LVL4_NONE };


    
    TEST_ASSERT_EQUAL(LICOR_FAIL,LI8x0_Validate_NoPump(&XML));
    

}

void test_LI8x0_Validate_NoH2O_should_ReturnValid_for_NoH2OCommand(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_820,
                        .LVL2 = LICOR_XML_LVL2_AUXDATA,
                        .LVL3 = LICOR_XML_LVL3_NONE,
                        .LVL4 = LICOR_XML_LVL4_NONE };


    
    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_Validate_NoH2O(&XML));
    

}


void test_LI8x0_Validate_NoH2O_should_ReturnInvalid_for_H2OCommand(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_820,
                        .LVL2 = LICOR_XML_LVL2_AUXDATA,
                        .LVL3 = LICOR_XML_LVL3_H2OABS,
                        .LVL4 = LICOR_XML_LVL4_NONE };


    
    TEST_ASSERT_EQUAL(LICOR_FAIL,LI8x0_Validate_NoH2O(&XML));
    

}


void test_LI8x0_CreateTags_should_ReturnValid_for_None(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_NONE,
                        .LVL2 = LICOR_XML_LVL2_NONE,
                        .LVL3 = LICOR_XML_LVL3_NONE,
                        .LVL4 = LICOR_XML_LVL4_NONE };

    char sstr[32];
    char *psstr = &sstr[0];
    char estr[32];
    char *pestr = &estr[0];
    memset(sstr,0,32);
    memset(estr,0,32);

    uint16_t slen=0;
    uint16_t elen=0;

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_CreateTags(&XML,psstr,&slen, pestr, &elen));
    TEST_ASSERT_EQUAL_STRING("",sstr);
    TEST_ASSERT_EQUAL_STRING("",estr);
    TEST_ASSERT_EQUAL(0,slen);
    TEST_ASSERT_EQUAL(0,elen);
}


void test_LIx80_CreateTags_should_ReturnValid_for_LI820_DATA_CO2_QUERY(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_820,
                        .LVL2 = LICOR_XML_LVL2_DATA,
                        .LVL3 = LICOR_XML_LVL3_CO2,
                        .LVL4 = LICOR_XML_LVL4_NONE };

    char sstr[32];
    char *psstr = &sstr[0];
    char estr[32];
    char *pestr = &estr[0];
    memset(sstr,0,32);
    memset(estr,0,32);

    uint16_t slen=0;
    uint16_t elen=0;

    char sExpectedResult[] = "<LI820><DATA><CO2>";
    char eExpectedResult[] = "</CO2></DATA></LI820>";

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_CreateTags(&XML,psstr,&slen,pestr,&elen));
    TEST_ASSERT_EQUAL_STRING(sExpectedResult,sstr);
    TEST_ASSERT_EQUAL_STRING(eExpectedResult,estr);

}


void test_LI8x0_CreateTags_should_ReturnValid_for_LI850_RS232_RAW_CO2(void){
    sLicorXML_t XML = { .LVL1 = LICOR_XML_LVL1_850,
                        .LVL2 = LICOR_XML_LVL2_RS232,
                        .LVL3 = LICOR_XML_LVL3_RAW,
                        .LVL4 = LICOR_XML_LVL4_CO2 };

    char sstr[32];
    char *psstr = &sstr[0];
    char estr[32];
    char *pestr = &estr[0];
    memset(sstr,0,32);
    memset(estr,0,32);

    uint16_t slen=0;
    uint16_t elen=0;

    char sExpectedResult[] = "<LI850><RS232><RAW><CO2>";
    char eExpectedResult[] = "</CO2></RAW></RS232></LI850>";

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_CreateTags(&XML,psstr,&slen,pestr,&elen));
    TEST_ASSERT_EQUAL_STRING(sExpectedResult,sstr);
    TEST_ASSERT_EQUAL_STRING(eExpectedResult,estr);


}


void test_LI8x0_IsAck_should_ReturnValid_for_ValidAck(void){

    sLicorXML_t XML = {   .LVL1 = LICOR_XML_LVL1_830,
                            .LVL2 = LICOR_XML_LVL2_ACK,
                            .LVL3 = LICOR_XML_LVL3_NONE,
                            .LVL4 = LICOR_XML_LVL4_NONE };

    char rstr[] = "<LI830><ACK>TRUE</ACK></LI830>\r\n";
    uint16_t rlen = strlen(rstr);

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_IsAck(&XML,rstr,rlen));


}

void test_LI8x0_IsAck_should_ReturnInvalid_for_Nack(void){
    sLicorXML_t XML = {   .LVL1 = LICOR_XML_LVL1_830,
                            .LVL2 = LICOR_XML_LVL2_ACK,
                            .LVL3 = LICOR_XML_LVL3_NONE,
                            .LVL4 = LICOR_XML_LVL4_NONE };

    char rstr[] = "<LI830><ACK>FALSE</ACK></LI830>\r\n";
    uint16_t rlen = strlen(rstr);

    TEST_ASSERT_EQUAL(LICOR_FAIL,LI8x0_IsAck(&XML,rstr,rlen));
}






void test_LI8X0_FindSubstring_should_ReturnHeadAndTailIndexes_Simple(void){
    char datamsg[] = "<li830><ack>True</ack></li830>";
    char teststr[] = "ack";
    uint32_t sIdx = 0;
    uint32_t eIdx = 0;


    TEST_ASSERT_EQUAL(LICOR_OK,LI8X0_FindSubstring(  datamsg,teststr,strlen(teststr),
                                                            &sIdx, &eIdx));

    TEST_ASSERT_EQUAL(12,sIdx);
    TEST_ASSERT_EQUAL(16,eIdx);

}


void test_LI8x0_FindSubstring_should_Fail_if_NoSubstringExists(void){

    char datamsg[] = "<li830><ack>True</ack></li830>";
    char teststr[] = "marsupial";
    uint32_t sIdx = 0;
    uint32_t eIdx = 0;


    TEST_ASSERT_EQUAL(LICOR_FAIL,LI8X0_FindSubstring(  datamsg,teststr,strlen(teststr),
                                                            &sIdx, &eIdx));

}


void test_LI8x0_ParseData_should_ParseDataIntoStruct_and_ReturnValid(void){
    sLicorTags_t Tags;
    float test_val_float = 3.1415827;
    uint32_t test_val_int = 12345;
    char datamsg[] = "<li830><data><flowrate>3.1415827</flowrate><celltemp>3.1415827</celltemp><cellpres>3.1415827</cellpres><co2>3.1415827</co2><co2abs>3.1415827</co2abs><h2o>3.1415827</h2o><h2oabs>3.1415827</h2oabs><h2odewpoint>3.1415827</h2odewpoint><ivolt>3.1415827</ivolt><raw><co2>12345</co2><co2ref>12345</co2ref><h2o>12345</h2o><h2oref>12345</h2oref></raw></data><li830>";
    uint32_t i=0;
    for(i=0;i<strlen(datamsg);i++){
        datamsg[i] = toupper(datamsg[i]);
    }

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ParseData(datamsg,strlen(datamsg),&Tags));
    TEST_ASSERT_EQUAL_FLOAT(test_val_float,Tags.data.celltemp);
    TEST_ASSERT_EQUAL_FLOAT(test_val_float,Tags.data.cellpres);
    TEST_ASSERT_EQUAL_FLOAT(test_val_float,Tags.data.flowrate);
    TEST_ASSERT_EQUAL_FLOAT(test_val_float,Tags.data.co2);
    TEST_ASSERT_EQUAL_FLOAT(test_val_float,Tags.data.co2abs);
    TEST_ASSERT_EQUAL_FLOAT(test_val_float,Tags.data.h2o);
    TEST_ASSERT_EQUAL_FLOAT(test_val_float,Tags.data.h2oabs);
    TEST_ASSERT_EQUAL_FLOAT(test_val_float,Tags.data.h2odewpoint);
    TEST_ASSERT_EQUAL_FLOAT(test_val_float,Tags.data.ivolt);

    TEST_ASSERT_EQUAL(test_val_int,Tags.data.raw.co2);
}


void test_LI8x0_ParseAuxdata_should_ParseAuxdataIntoStruct_and_ReturnValid(void){
    sLicorTags_t Tags;
    float test_val_float = 3.1415827;
    uint32_t test_val_int = 12345;
    char datamsg[] = "<li830><auxdata><pca>3.1415827</pca><pha>3.1415827</pha><bb_eff>3.1415827</bb_eff><psi>3.1415827</psi>";
    uint32_t i=0;
    for(i=0;i<strlen(datamsg);i++){
        datamsg[i] = toupper(datamsg[i]);
    }

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ParseAuxdata(datamsg,strlen(datamsg),&Tags));
    
    TEST_ASSERT_EQUAL_FLOAT(test_val_float,Tags.auxdata.pha);
    TEST_ASSERT_EQUAL_FLOAT(test_val_float,Tags.auxdata.pca);
    TEST_ASSERT_EQUAL_FLOAT(test_val_float,Tags.auxdata.bb_eff);
    TEST_ASSERT_EQUAL_FLOAT(test_val_float,Tags.auxdata.psi);

}


void test_LI8x0_ParseRS232_should_ParseRS232IntoStruct_and_ReturnValid(void){
    sLicorTags_t Tags;
    float test_val_float = 3.1415827;
    uint32_t test_val_int = 12345;
    char datamsg[] = "<li830><rs232><flowrate>True</flowrate><celltemp>True</celltemp><cellpres>True</cellpres><co2>True</co2><co2abs>True</co2abs><h2o>True</h2o><h2oabs>True</h2oabs><h2odewpoint>True</h2odewpoint><ivolt>True</ivolt><raw><co2>True</co2><co2ref>True</co2ref><h2o>True</h2o><h2oref>True</h2oref></raw></rs232><li830>";
    uint32_t i=0;
    for(i=0;i<strlen(datamsg);i++){
        datamsg[i] = toupper(datamsg[i]);
    }

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ParseRS232(datamsg,strlen(datamsg),&Tags));
    TEST_ASSERT_EQUAL(true,Tags.rs232.celltemp);
    TEST_ASSERT_EQUAL(true,Tags.rs232.cellpres);
    TEST_ASSERT_EQUAL(true,Tags.rs232.flowrate);
    TEST_ASSERT_EQUAL(true,Tags.rs232.co2);
    TEST_ASSERT_EQUAL(true,Tags.rs232.co2abs);
    TEST_ASSERT_EQUAL(true,Tags.rs232.h2o);
    TEST_ASSERT_EQUAL(true,Tags.rs232.h2oabs);
    TEST_ASSERT_EQUAL(true,Tags.rs232.h2odewpoint);
    TEST_ASSERT_EQUAL(true,Tags.rs232.ivolt);

    TEST_ASSERT_EQUAL(true,Tags.rs232.raw.co2);

}


void test_LI8x0_ParseCFG_should_ParseCFGIntoStruct_and_ReturnValid(void){
    sLicorTags_t Tags;
    float test_val_float = 1.2345;
    uint32_t test_val_int = 12345;
    char datamsg[] = "<LI830><CFG><HEATER>TRUE</HEATER><PCOMP>TRUE</PCOMP><FILTER>20</FILTER><BENCH>14</BENCH><SPAN>123</SPAN><ALARMS><LOGIC>TTL</LOGIC><SOURCE>CO2</SOURCE><ENABLED>TRUE</ENABLED><HIGH>1.2345</HIGH><HDEAD>1.2345</HDEAD><LOW>1.2345</LOW><LDEAD>1.2345</LDEAD></ALARMS><DACS><RANGE>2.5</RANGE><D1>CO2</D1><D1_0>1.2345</D1_0><D1_F>1.2345</D1_F><D2>CO2</D2><D2_0>1.2345</D2_0><D2_F>1.2345</D2_F></DACS></CFG></LI830>";
    uint32_t i=0;
    for(i=0;i<strlen(datamsg);i++){
        datamsg[i] = toupper(datamsg[i]);
    }

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ParseCFG(datamsg,strlen(datamsg),&Tags));
    TEST_ASSERT_EQUAL(true,Tags.cfg.heater);
    TEST_ASSERT_EQUAL(true,Tags.cfg.pcomp);
    TEST_ASSERT_EQUAL(20,Tags.cfg.filter);     
    TEST_ASSERT_EQUAL(14,Tags.cfg.bench);   
    TEST_ASSERT_EQUAL(123,Tags.cfg.span);
    TEST_ASSERT_EQUAL(TTL,Tags.cfg.alarms.logic);
    TEST_ASSERT_EQUAL(CO2,Tags.cfg.alarms.sources);
    TEST_ASSERT_EQUAL(true,Tags.cfg.alarms.enabled);
    TEST_ASSERT_EQUAL(test_val_float,Tags.cfg.alarms.high);
    TEST_ASSERT_EQUAL(test_val_float,Tags.cfg.alarms.hdead);
    TEST_ASSERT_EQUAL(test_val_float,Tags.cfg.alarms.low);
    TEST_ASSERT_EQUAL(test_val_float,Tags.cfg.alarms.ldead);
    TEST_ASSERT_EQUAL(LICOR_DAC_CO2,Tags.cfg.dacs.d1);
    TEST_ASSERT_EQUAL(test_val_float,Tags.cfg.dacs.d1_0);
    TEST_ASSERT_EQUAL(test_val_float,Tags.cfg.dacs.d1_f);
    TEST_ASSERT_EQUAL(LICOR_DAC_CO2,Tags.cfg.dacs.d2);
    TEST_ASSERT_EQUAL(test_val_float,Tags.cfg.dacs.d2_0);
    TEST_ASSERT_EQUAL(test_val_float,Tags.cfg.dacs.d2_f);
}

void test_LI8x0_ParsePoly_should_ParsePolyIntoStruct_and_ReturnValid(void){
    sLicorTags_t Tags;
    float test_val_float = 1.2345;
    uint32_t test_val_int = 12345;
    char datamsg[] = "<li840><poly><date>2018-02-13</date><bb>1.2345</bb><xs>1.2345</xs><reset>True</reset><co2><a1>1.2345</a1><a2>1.2345</a2><a3>1.2345</a3><a4>1.2345</a4></co2><h2o><a1>1.2345</a1><a2>1.2345</a2><a3>1.2345</a3></h2o><pres><a0>1.2345</a0><a1>1.2345</a1></pres></poly></li840>";    
    uint32_t i=0;
    for(i=0;i<strlen(datamsg);i++){
        datamsg[i] = toupper(datamsg[i]);
    }

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ParsePoly(datamsg,strlen(datamsg),&Tags));
    TEST_ASSERT_EQUAL_STRING("2018-02-13",Tags.poly.date);
    TEST_ASSERT_EQUAL(true,Tags.poly.reset);
    TEST_ASSERT_EQUAL(test_val_float,Tags.poly.bb);
    TEST_ASSERT_EQUAL(test_val_float,Tags.poly.xs);    
    TEST_ASSERT_EQUAL(test_val_float,Tags.poly.co2.a1);
    TEST_ASSERT_EQUAL(test_val_float,Tags.poly.co2.a2);   
    TEST_ASSERT_EQUAL(test_val_float,Tags.poly.co2.a3);
    TEST_ASSERT_EQUAL(test_val_float,Tags.poly.co2.a4);   
    TEST_ASSERT_EQUAL(test_val_float,Tags.poly.h2o.a1);
    TEST_ASSERT_EQUAL(test_val_float,Tags.poly.h2o.a1);   
    TEST_ASSERT_EQUAL(test_val_float,Tags.poly.h2o.a1);
    TEST_ASSERT_EQUAL(test_val_float,Tags.poly.pres.a0);   
    TEST_ASSERT_EQUAL(test_val_float,Tags.poly.pres.a1);

}

void test_LI8x0_ParsePump_should_ParsePumpIntoStruct_and_ReturnValid(void){
    sLicorTags_t Tags;
    float test_val_float = 1.2345;
    uint32_t test_val_int = 12345;
    char datamsg[] = "<li830><pump><enabled>True</enabled><time>12345678</time><drive>123</drive><status>1</status></pump></li830>";    
    uint32_t i=0;
    for(i=0;i<strlen(datamsg);i++){
        datamsg[i] = toupper(datamsg[i]);
    }

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ParsePump(datamsg,strlen(datamsg),&Tags));
    TEST_ASSERT_EQUAL(true,Tags.pump.enabled);
    TEST_ASSERT_EQUAL(12345678,Tags.pump.time);
    TEST_ASSERT_EQUAL(123,Tags.pump.drive);
    TEST_ASSERT_EQUAL(LICOR_FLOW_ONLY,Tags.pump.status);

}

void test_LI8x0_ParseSource_should_ParseSourceIntoStruct_and_ReturnValid(void){
    sLicorTags_t Tags;
    float test_val_float = 1.2345;
    uint32_t test_val_int = 12345;
    char datamsg[] = "<li830><source><time>12345678</time></source></li830>";    
    uint32_t i=0;
    for(i=0;i<strlen(datamsg);i++){
        datamsg[i] = toupper(datamsg[i]);
    }

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ParseSource(datamsg,strlen(datamsg),&Tags));
    TEST_ASSERT_EQUAL(12345678,Tags.source.time);

}


void test_LI8x0_ParseTech_should_ParseTechIntoStruct_and_ReturnValid(void){
    sLicorTags_t Tags;
    float test_val_float = 1.2345;
    uint32_t test_val_int = 12345;
    char datamsg[] = "<li850><tech><pump><model>ABCDEF</model><ki>12345</ki><kp>12345</kp><kd>12345</kd><offset>12345</offset><gain>12345</gain><flow>1.2345</flow></pump><source><voltage>1.2345</voltage></source></tech></li850>";    
    uint32_t i=0;
    for(i=0;i<strlen(datamsg);i++){
        datamsg[i] = toupper(datamsg[i]);
    }

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ParseTech(datamsg,strlen(datamsg),&Tags));
    TEST_ASSERT_EQUAL(test_val_float,Tags.tech.source.voltage);
    TEST_ASSERT_EQUAL(test_val_float,Tags.tech.pump.flow);
    TEST_ASSERT_EQUAL(test_val_int,Tags.tech.pump.ki);
    TEST_ASSERT_EQUAL(test_val_int,Tags.tech.pump.kp);
    TEST_ASSERT_EQUAL(test_val_int,Tags.tech.pump.kd);
    TEST_ASSERT_EQUAL(test_val_int,Tags.tech.pump.gain);
    TEST_ASSERT_EQUAL(test_val_int,Tags.tech.pump.offset);
}


void test_LI8x0_ParseString_should_ParseAck(void){
    sLicorTags_t Tags;
    float test_val_float = 1.2345;
    uint32_t test_val_int = 12345;   
    uint32_t i=0;
    char datamsg[] = "<li850><ack>True</ack></li850>"; 

    for(i=0;i<strlen(datamsg);i++){
        datamsg[i] = toupper(datamsg[i]);
    }

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ParseString(datamsg,strlen(datamsg),&Tags));
    TEST_ASSERT_EQUAL(true,Tags.ack);

}

void test_LI8x0_ParseString_should_ParseVer(void){
    sLicorTags_t Tags;
    float test_val_float = 1.2345;
    uint32_t test_val_int = 12345;   
    uint32_t i=0;
    
    char datamsg[] = "<li850><ver>LICORV1234</ver></li850>"; 
    for(i=0;i<strlen(datamsg);i++){
        datamsg[i] = toupper(datamsg[i]);
    }

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ParseString(datamsg,strlen(datamsg),&Tags));
    TEST_ASSERT_EQUAL_STRING("LICORV1234",Tags.ver);

}

void test_LI8x0_ParseString_should_ParseError(void){
    sLicorTags_t Tags;
    float test_val_float = 1.2345;
    uint32_t test_val_int = 12345;   
    uint32_t i=0;
    
    char datamsg[] = "<li850><error>There Is An Error</error></li850>"; 
    for(i=0;i<strlen(datamsg);i++){
        datamsg[i] = toupper(datamsg[i]);
    }

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ParseString(datamsg,strlen(datamsg),&Tags));
    TEST_ASSERT_EQUAL_STRING("THERE IS AN ERROR",Tags.error);

}

void test_LI8x0_ParseString_should_ParseAll(void){
    sLicorTags_t Tags;
    float test_val_float = 1.2345;
    uint32_t test_val_int = 123;   
    uint32_t i=0;
    
    char datamsg[] = "<li850><ack>True</ack><ver>ABCDEF</ver><error>ABCDEF</error><data><flowrate>1.2345</flowrate><celltemp>1.2345</celltemp><cellpres>1.2345</cellpres><co2>1.2345</co2><co2abs>1.2345</co2abs><h2o>1.2345</h2o><h2oabs>1.2345</h2oabs><h2odewpoint>1.2345</h2odewpoint><ivolt>1.2345</ivolt><raw><co2>123</co2><co2ref>123</co2ref><h2o>123</h2o><h2oref>123</h2oref></raw></data><auxdata><pca>1.2345</pca><pha>1.2345</pha><bb_eff>1.2345</bb_eff><psi>1.2345</psi></auxdata><rs232><flowrate>True</flowrate><co2>True</co2><h2o>True</h2o><celltemp>True</celltemp><cellpres>True</cellpres><ivolt>True</ivolt><co2abs>True</co2abs><h2oabs>True</h2oabs><h2odewpoint>True</h2odewpoint><raw><co2>True</co2><co2ref>True</co2ref><h2o>True</h2o><h2oref>True</h2oref></raw><echo>True</echo><strip>True</strip></rs232><cfg><heater>True</heater><pcomp>True</pcomp><filter>123</filter><outrate>1.2345</outrate><alarms><logic>ttl</logic><source>co2<source><enabled>True</enabled><high>1.2345</high><hdead>1.2345</hdead><low>1.2345</low><ldead>1.2345</ldead></alarms><bench>14</bench><span>123</span><dacs><range>5.0</range><d1>none</d1><d1_0>1.2345</d1_0><d1_f>1.2345</d1_f><d2>none</d2><d2_0>1.2345</d2_0><d2_0>1.2345</d2_f></dacs></cfg><cal><co2lastzero>02-13-2018</co2lastzero><co2kzero>1.2345</co2kzero><co2lastspan>02-13-2018</co2lastspan><co2lastspan2>02-13-2018</co2lastspan2><co2kspan>1.2345</co2kspan><co2kspan2>1.2345</co2kspan2><h2olastzero>02-13-2018</h2olastzero><h2okzero>True</h2okzero><h2olastspan>02-13-2018</h2olastspan><h2olastspan2>02-13-2018</h2olastspan2><h2okspan>1.2345</h2okspan><h2okspan2>1.2345</h2okspan2></cal><poly><date>ABCDEF</date><bb>1.2345</bb><xs>1.2345</xs><reset>True</reset><co2><a1>1.2345</a1><a2>1.2345</a2><a3>1.2345</a3><a4>1.2345</a4></co2><h2o><a1>1.2345</a1><a2>1.2345</a2><a3>1.2345</a3></h2o><press><a0>1.2345</a0><a1>1.2345</a1></press></poly><pump><enabled>True</enabled><time>123</time><drive>123</drive><status>123</status></pump><source><time>123</time></source><tech><pump><model>ABCDEF</model><ki>123</ki><kp>123</kp><kd>123</kd><offset>123</offset><gain>123</gain><flow>1.2345</flow></pump><source><voltage>1.2345</voltage></source></tech><serialnum>ABCDEF</serialnum></li850>"; 
    for(i=0;i<strlen(datamsg);i++){
        datamsg[i] = toupper(datamsg[i]);
    }

    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_ParseString(datamsg,strlen(datamsg),&Tags));
    TEST_ASSERT_EQUAL(test_val_float,Tags.data.flowrate);
    TEST_ASSERT_EQUAL(test_val_float,Tags.auxdata.pca);
    TEST_ASSERT_EQUAL(true,Tags.rs232.co2);
    TEST_ASSERT_EQUAL(true,Tags.cfg.heater);
    TEST_ASSERT_EQUAL(test_val_float,Tags.cal.co2kzero);
    TEST_ASSERT_EQUAL(test_val_float,Tags.poly.bb);
    TEST_ASSERT_EQUAL(test_val_int,Tags.pump.drive);
    TEST_ASSERT_EQUAL(test_val_int,Tags.source.time);
    TEST_ASSERT_EQUAL(test_val_float,Tags.tech.source.voltage);
    TEST_ASSERT_EQUAL_STRING("ABCDEF", Tags.serialnum);
}

void test_LI8x0_Zero_should_RunZero_and_ReturnValid(void){
    AM08X5_Datestamp_t dstmp = DS_YYYY_MM_DD;
    AM08X5_Timestamp_t tstmp = TS_TIME_NONE;
    
    char sstr[32];
    char *psstr = &sstr[0];
    char estr[32];
    char *pestr = &estr[0];
    // char 
    char rstr[80];
    memset(sstr,0,32);
    memset(estr,0,32);
    memset(rstr,0,80);

    uint16_t slen=0;
    uint16_t elen=0;
    uint16_t rlen=0;


    char ackStr[] = "<LI830><ACK>TRUE</ACK></LI830>\r\n";
    char expectedMsg[80] = "<LI830><CAL><DATE>2018-02-05</DATE><CO2ZERO>TRUE</CO2ZERO></CAL></LI830>";
    char dtstr[] = "2018-02-05";
    char tstr[16];
    memset(tstr,0,16);
    char retStr[] = "<li830><cal><co2lastzero>2018-02-05</co2lastzero><co2kzero>1.2345</co2kzero></cal></li830>\r\n";

    /** Update Buffer with test string for ACK */
    BufferC_puts(&pLicor1->UART.buf,ackStr,strlen(ackStr));
    BufferC_puts(&pLicor1->UART.buf,retStr,strlen(retStr));

    /** Mock Get Date */
    AM08X5_GetTimestamp_Expect(tstr,dstmp,tstmp);
    AM08X5_GetTimestamp_IgnoreArg_str();
    AM08X5_GetTimestamp_ReturnArrayThruPtr_str(dtstr,strlen(dtstr));
   

    /** Mock Send UART String */
    BSP_UART_puts_Expect(&pLicor1->UART,expectedMsg,strlen(expectedMsg));

    /** Mock Get Response */
    

    /** Mock Verify Delay Request */
    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_Zero());
    TEST_ASSERT_EQUAL_STRING("2018-02-05",pLicor1->readconfig.cal.co2lastzero);
}



void test_LI8x0_Span_should_RunSpan_and_ReturnValid(void){
    AM08X5_Datestamp_t dstmp = DS_YYYY_MM_DD;
    AM08X5_Timestamp_t tstmp = TS_TIME_NONE;
    

    char sstr[32];
    char *psstr = &sstr[0];
    char estr[32];
    char *pestr = &estr[0];
    // char 
    char rstr[80];
    memset(sstr,0,32);
    memset(estr,0,32);
    memset(rstr,0,80);

    uint16_t slen=0;
    uint16_t elen=0;
    uint16_t rlen=0;


    char ackStr[] = "<LI830><ACK>TRUE</ACK></LI830>\r\n";
    char expectedMsg[80] = "<LI830><CAL><DATE>2018-02-05</DATE><CO2ZERO>TRUE</CO2ZERO></CAL></LI830>";
    char dtstr[] = "2018-02-05";
    char tstr[16];
    memset(tstr,0,16);
    char retStr[] = "<li830><cal><co2lastzero>2018-02-05</co2lastzero><co2kzero>1.2345</co2kzero></cal></li830>\r\n";

    /** Update Buffer with test string for ACK */
    BufferC_puts(&pLicor1->UART.buf,ackStr,strlen(ackStr));
    BufferC_puts(&pLicor1->UART.buf,retStr,strlen(retStr));

    /** Mock Get Date */
    AM08X5_GetTimestamp_Expect(tstr,dstmp,tstmp);
    AM08X5_GetTimestamp_IgnoreArg_str();
    AM08X5_GetTimestamp_ReturnArrayThruPtr_str(dtstr,strlen(dtstr));
   

    /** Mock Send UART String */
    BSP_UART_puts_Expect(&pLicor1->UART,expectedMsg,strlen(expectedMsg));

    /** Mock Get Response */
    

    /** Mock Verify Delay Request */
    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_Zero());
    TEST_ASSERT_EQUAL_STRING("2018-02-05",pLicor1->readconfig.cal.co2lastzero);
}



void test_LI8x0_ClearTags_should_ClearAllTags(void){
    sLicorTags_t Tags ={.pump = {.drive = 6}};
    
    LI8X0_ClearTags(&Tags);
    TEST_ASSERT_EQUAL(0,Tags.pump.drive);
}



void test_LI8x0_GetInfo_should_UpdateStruct(void){
    AM08X5_Datestamp_t dstmp = DS_YYYY_MM_DD;
    AM08X5_Timestamp_t tstmp = TS_TIME_NONE;

    char sstr[32];
    char *psstr = &sstr[0];
    char estr[32];
    char *pestr = &estr[0];
    // char 
    char rstr[80];
    memset(sstr,0,32);
    memset(estr,0,32);
    memset(rstr,0,80);

    uint16_t slen=0;
    uint16_t elen=0;
    uint16_t rlen=0;


    char ackStr[] = "<LI830><ACK>TRUE</ACK></LI830>\r\n";
    char expectedMsg[80] = "<LI830>?</LI830>";
    char dtstr[] = "2018-02-05";
    char tstr[16];
    memset(tstr,0,16);
    char retStr[] = "<li830><ack>True</ack><ver>ABCDEF</ver><error>ABCDEF</error><data><flowrate>1.2345</flowrate><celltemp>1.2345</celltemp><cellpres>1.2345</cellpres><co2>1.2345</co2><co2abs>1.2345</co2abs><h2o>1.2345</h2o><h2oabs>1.2345</h2oabs><h2odewpoint>1.2345</h2odewpoint><ivolt>1.2345</ivolt><raw><co2>123</co2><co2ref>123</co2ref><h2o>123</h2o><h2oref>123</h2oref></raw></data><auxdata><pca>1.2345</pca><pha>1.2345</pha><bb_eff>1.2345</bb_eff><psi>1.2345</psi></auxdata><rs232><flowrate>True</flowrate><co2>True</co2><h2o>True</h2o><celltemp>True</celltemp><cellpres>True</cellpres><ivolt>True</ivolt><co2abs>True</co2abs><h2oabs>True</h2oabs><h2odewpoint>True</h2odewpoint><raw><co2>True</co2><co2ref>True</co2ref><h2o>True</h2o><h2oref>True</h2oref></raw><echo>True</echo><strip>True</strip></rs232><cfg><heater>True</heater><pcomp>True</pcomp><filter>123</filter><outrate>1.2345</outrate><alarms><logic>ttl</logic><source>co2<source><enabled>True</enabled><high>1.2345</high><hdead>1.2345</hdead><low>1.2345</low><ldead>1.2345</ldead></alarms><bench>14</bench><span>123</span><dacs><range>5.0</range><d1>none</d1><d1_0>1.2345</d1_0><d1_f>1.2345</d1_f><d2>none</d2><d2_0>1.2345</d2_0><d2_0>1.2345</d2_f></dacs></cfg><cal><co2lastzero>02-13-2018</co2lastzero><co2kzero>1.2345</co2kzero><co2lastspan>02-13-2018</co2lastspan><co2lastspan2>02-13-2018</co2lastspan2><co2kspan>1.2345</co2kspan><co2kspan2>1.2345</co2kspan2><h2olastzero>02-13-2018</h2olastzero><h2okzero>True</h2okzero><h2olastspan>02-13-2018</h2olastspan><h2olastspan2>02-13-2018</h2olastspan2><h2okspan>1.2345</h2okspan><h2okspan2>1.2345</h2okspan2></cal><poly><date>ABCDEF</date><bb>1.2345</bb><xs>1.2345</xs><reset>True</reset><co2><a1>1.2345</a1><a2>1.2345</a2><a3>1.2345</a3><a4>1.2345</a4></co2><h2o><a1>1.2345</a1><a2>1.2345</a2><a3>1.2345</a3></h2o><press><a0>1.2345</a0><a1>1.2345</a1></press></poly><pump><enabled>True</enabled><time>123</time><drive>123</drive><status>123</status></pump><source><time>123</time></source><tech><pump><model>ABCDEF</model><ki>123</ki><kp>123</kp><kd>123</kd><offset>123</offset><gain>123</gain><flow>1.2345</flow></pump><source><voltage>1.2345</voltage></source></tech><serialnum>ABCDEF</serialnum></li830>\r\n"; 
    
    pLicor1->type = LICOR_LI830;

    /** Update Buffer with test string for ACK */
    BufferC_puts(&pLicor1->UART.buf,ackStr,strlen(ackStr));
    BufferC_puts(&pLicor1->UART.buf,retStr,strlen(retStr));
      

    /** Mock Send UART String */
    BSP_UART_puts_Expect(&pLicor1->UART,expectedMsg,strlen(expectedMsg));


    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_GetInfo());
    TEST_ASSERT_EQUAL_STRING("ABCDEF",pLicor1->sysconfig.ver);
}



// void test_LI8x0_SetLicor_should_SetOutputs(void){



// }

void test_LI8x0_PowerON_should_TurnLicorON(void){
    /* Arrange */

    /* Expect */
    BSP_GPIO_SetPin_Expect(&pLicor1->Power);

    /* Act */
    LI8x0_PowerON();
}

void test_LI8x0_PowerOFF_should_TurnLicorOFF(void){
    /* Arrange */

    /* Expect */
    BSP_GPIO_ClearPin_Expect(&pLicor1->Power);

    /* Act */
    LI8x0_PowerOFF();
}

void test_LI8x0_HeaterON_should_send_heateron_command(void){
    /* Arrange */
    pLicor1->type = LICOR_LI820;
    char bspstr[] = "<LI820><CFG><HEATER>TRUE</HEATER></CFG></LI820>";
    char ack[] = "<LI820><ACK>TRUE</ACK></LI820>\r\n";
    uint8_t state = 1;

    BufferC_puts(&pLicor1->UART.buf,ack,strlen(ack));

    /* Expect */
    BSP_UART_puts_Expect(&pLicor1->UART,bspstr,strlen(bspstr));
    strcpy(LicorBuffer,ack);


    /* Act */
    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_Heater(state));
}

void test_LI8x0_HeaterOFF_should_send_heateron_command(void){
    /* Arrange */
    pLicor1->type = LICOR_LI820;
    char bspstr[] = "<LI820><CFG><HEATER>FALSE</HEATER></CFG></LI820>";
    char ack[] = "<LI820><ACK>TRUE</ACK></LI820>\r\n";
    uint8_t state = 0;

    BufferC_puts(&pLicor1->UART.buf,ack,strlen(ack));
    /* Expect */
    BSP_UART_puts_Expect(&pLicor1->UART,bspstr,strlen(bspstr));
    strcpy(LicorBuffer,ack);
    // /* Act */
    TEST_ASSERT_EQUAL(LICOR_OK,LI8x0_Heater(state));
}

// // void test_LI8x0_Config_should_set_all_parameters(void){
// //     /** Arrange */
// //     pLicor1->type = LICOR_LI830;
// //     char sstr[80];

// //     /** Expect */
// //     /* **** RS232 **** */
// //     /* FlowRate = Off */
// //     /* Co2 = On */
// //     /* h2o = Ignored */
// //     /* celltemp = On */
// //     /* cellpres = On */
// //     /* ivolt = On */
// //     /* co2abs = On */
// //     /* h2oabs = Ignored */
// //     /* raw_co2 = On */
// //     /* raw_co2ref = On */
// //     /* raw_h20 = Ignored */
// //     /* raw_h20ref = Ignored */
// //     /* echo = Off */
// //     /* strip = On */
    
// //     memset(sstr,0,80);
// //     strcpy(sstr, "<LI830><RS232><FLOWRATE>FALSE</FLOWRATE></RS232></LI830>");
// //     printf("%s\n",sstr);
    

// //     memset(sstr,0,80);
// //     strcpy(sstr,"LI830><RS232><CO2>TRUE</CO2></RS232></LI830>");
// //     printf("%s\n",sstr);
// //     BSP_UART_puts_Expect(&pLicor1->UART,sstr,strlen(sstr));

// //     // BSP_UART_puts_Expect(&pLicor1->UART,sstr,strlen(sstr));
// //     // BSP_UART_puts_Expect(&pLicor1->UART,"<LI830><RS232><CO2>False</LI830></RS232></FLOWRATE>")


// //     /* **** CFG **** */
// //     /* heater = Off */
// //     /* pcomp = On */
// //     /* filter = 0 */
// //     /* outrate = 0 */
// //     /* bench = 14 */
// //     /* span = SPAN */
// //     /* alarm_enabled = Off */

// //     /** Act */
// //     LI8x0_Config();
// // }