
#define ADDR_TPS 0x074


// Registres
#define REG_REF_LSB 0
#define REG_REF_MSB 1
#define REG_IOUT_LIMIT 2
#define REG_VOUT_SR 3
#define REG_VOUT_FS 4
#define REG_CDC 5
#define REG_MODE 6
#define REG_STATUS 7

// Bits de configuration
#define TPS_OE_ON 0x80
#define TPS_FBR_2 0x02
#define TPS_IOUT_LIMIT_ENABLE 0x80


// Limites de courant
#define TPS_IOUT_LIMIT_1_0A 0x14
#define TPS_IOUT_LIMIT_1_2A 0x18
#define TPS_IOUT_LIMIT_1_5A 0x1E
#define TPS_IOUT_LIMIT_2A 0x28
#define TPS_IOUT_LIMIT_MAX 0x7F


#define TPS_STATE_OFF 0
#define TPS_STATE_ON 1 



struct TPS 
{
    uint16_t VREF;
    uint8_t I_OUT_LIMIT;
    uint8_t VOUT_SR;
    uint8_t VOUT_FS;
    uint8_t CDC;
    uint8_t MODE;
    uint8_t STATUS;
    
    uint8_t OE;
    uint8_t OCP;
    uint8_t SCP;
};


void TPS55288_Init(uint8_t SlaveAdr);


void TPS55288_Read_ALL_REG(uint8_t SlaveAdr);
void TPS55288_SetVout(uint8_t SlaveAdr, float vout);
void TPS55288_Write_VREF(uint8_t SlaveAdr, uint16_t vref);
void TPS55288_Read_STATUS(uint8_t SlaveAdr);


void TPS55288_W_REG(uint8_t SlaveAdr, uint8_t RegAdr, uint8_t RegVal);
uint8_t TPS55288_R_REG(uint8_t SlaveAdr, uint8_t RegAdr);
