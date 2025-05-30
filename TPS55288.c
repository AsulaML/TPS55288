#include "main.h"
#include "devices.h"
#include "TPS55288.h"
#include "I2C.h"
#include "board.h"
#include "mcc_generated_files/adc1.h"

struct TPS BuckBoost;

/**
 * \fn void TPS55288_SetVout(uint8_t SlaveAdr, float vout)
 * \brief fonction qui permet de convertir la tension en val registre
 * et de l'envoyer au composant pour changer sa consigne de tension
 * \return 0
 */
void TPS55288_SetVout(uint8_t SlaveAdr, float vout)
{
    uint16_t RegVref = 0;
    
    RegVref = (vout-0.6)/0.015;
    
    TPS55288_Write_VREF(SlaveAdr, RegVref);
}


/**
 * \fn void TPS55288_Init(uint8_t SlaveAdr)
 * \brief fonction qui permet d'initialiser le composant
 * \return 0
 */
void TPS55288_Init(uint8_t SlaveAdr)
{   
    // clear bot OCP_MASK and SCP MASK
    TPS55288_W_REG(SlaveAdr, REG_CDC, BuckBoost.CDC & 0x3F); //BFs
    BuckBoost.CDC = TPS55288_R_REG(SlaveAdr, REG_MODE);
    
    // Config de la limite de courant
    BuckBoost.I_OUT_LIMIT = TPS_IOUT_LIMIT_1_5A;
    BuckBoost.I_OUT_LIMIT |= TPS_IOUT_LIMIT_ENABLE; 
    TPS55288_W_REG(SlaveAdr, REG_IOUT_LIMIT, BuckBoost.I_OUT_LIMIT);
    BuckBoost.I_OUT_LIMIT = TPS55288_R_REG(SlaveAdr, REG_IOUT_LIMIT);
    
    // Activation de la sortie
    TPS55288_W_REG(SlaveAdr, REG_MODE, TPS_OE_ON);
    BuckBoost.MODE = TPS55288_R_REG(SlaveAdr, REG_MODE);
    
    // Configuration feedback et slew rate
    TPS55288_W_REG(SlaveAdr, REG_VOUT_FS, TPS_FBR_2);
    TPS55288_W_REG(SlaveAdr, REG_VOUT_SR, 3);
    
    // set bot OCP_MASK
    TPS55288_W_REG(SlaveAdr, REG_CDC, BuckBoost.CDC |0x40);
}

/**
 * \fn void TPS55288_Write_VREF(uint8_t SlaveAdr, uint16_t vref)
 * \brief fonction qui permet d'�crire dans la consigne de tension
 * \return 0
 */
void TPS55288_Write_VREF(uint8_t SlaveAdr, uint16_t vref)
{
    uint8_t H,L;
    
    vref = vref & 0x3FF;
            
    H = vref >> 8;
    L = vref & 0xFF;
    
    TPS55288_W_REG(SlaveAdr, REG_REF_LSB, L);
    BuckBoost.VREF = TPS55288_R_REG(SlaveAdr, REG_REF_LSB);
    
    TPS55288_W_REG(SlaveAdr, REG_REF_MSB, H);
    BuckBoost.VREF |= (uint16_t)(TPS55288_R_REG(SlaveAdr, REG_REF_MSB)<<8);
}

/**
 * \fn void TPS55288_Read_STATUS(uint8_t SlaveAdr)
 * \brief fonction qui permet de lire le registre de status
 * le r�sultat est stock� dans la structure
 * \return 0
 */
void TPS55288_Read_STATUS(uint8_t SlaveAdr)
{
    BuckBoost.STATUS = TPS55288_R_REG(SlaveAdr, REG_MODE);
}

/**
 * \fn void TPS55288_Read_ALL_REG(uint8_t SlaveAdr)
 * \brief fonction qui permet de lire l'ensemble des registres du composant
 * le r�sultat est stock� dans la structure
 * \return 0
 */
void TPS55288_Read_ALL_REG(uint8_t SlaveAdr)
{
    BuckBoost.VREF = TPS55288_R_REG(SlaveAdr, REG_REF_LSB);
    BuckBoost.VREF |= (uint16_t)(TPS55288_R_REG(SlaveAdr, REG_REF_MSB)<<8);
    BuckBoost.I_OUT_LIMIT = TPS55288_R_REG(SlaveAdr, REG_IOUT_LIMIT);
    BuckBoost.VOUT_SR = TPS55288_R_REG(SlaveAdr, REG_VOUT_SR);
    BuckBoost.VOUT_FS = TPS55288_R_REG(SlaveAdr, REG_VOUT_FS);
    BuckBoost.CDC = TPS55288_R_REG(SlaveAdr, REG_CDC);
    BuckBoost.MODE = TPS55288_R_REG(SlaveAdr, REG_MODE);
    BuckBoost.STATUS = TPS55288_R_REG(SlaveAdr, REG_STATUS);
    
    BuckBoost.OCP = (BuckBoost.STATUS >> 6) & 0x01;
    BuckBoost.SCP = (BuckBoost.STATUS >> 7) & 0x01;
    BuckBoost.OE = (BuckBoost.MODE >> 7) & 0x01;
}

/**
 * \fn void TPS55288_W_REG(uint8_t SlaveAdr, uint8_t RegAdr, uint8_t RegVal)
 * \brief fonction qui permet d'�crire les registres du composant
 * \return uint8_t
 */
void TPS55288_W_REG(uint8_t SlaveAdr, uint8_t RegAdr, uint8_t RegVal)
{
    I2C_Start();
    
    I2C_Write((SlaveAdr << 1));
    I2C_Write(RegAdr);
    
    I2C_Write(RegVal);
    I2C_Stop();
}

/**
 * \fn uint8_t TPS55288_R_REG(uint8_t SlaveAdr, uint8_t RegAdr)
 * \brief fonction qui permet de lire les registres du composant
 * \return uint8_t
 */
uint8_t TPS55288_R_REG(uint8_t SlaveAdr, uint8_t RegAdr)
{
    uint8_t tmp = 0;
    uint8_t val;
    
    I2C_Start();
    
    I2C_Write((SlaveAdr << 1));
    I2C_Write(RegAdr);
    
    I2C_Restart();
    tmp = (SlaveAdr << 1) | 0x01;
    I2C_Write(tmp);
    
    val = I2C_Read();
    I2C_NACK();
    
    I2C_Stop();
    
    return(val);
}