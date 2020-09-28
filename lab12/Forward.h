/*
    File:   Forward.h 
    Desc:   Declares Forward class and associated constants
 */
#ifndef FORWARD_H
#define FORWARD_H
#include <cstdint>

class Forward
{
    uint64_t e_dstE;
    uint64_t e_valE;
    uint64_t M_dstM;
    uint64_t m_valM;
    uint64_t M_dstE;
    uint64_t M_valE;
    uint64_t W_dstE;
    uint64_t W_valE;
    uint64_t W_dstM;
    uint64_t W_valM;
    uint64_t m_stat;
    uint64_t W_stat;
    uint64_t M_icode;
    bool M_Cnd;
    uint64_t M_valA;
    uint64_t W_icode;

    public:
        uint64_t gete_dstE(){return e_dstE;}
        uint64_t gete_valE(){return e_valE;}
        uint64_t getM_dstM(){return M_dstM;}
        uint64_t getm_valM(){return m_valM;}
        uint64_t getM_dstE(){return M_dstE;}
        uint64_t getM_valE(){return M_valE;}
        uint64_t getW_dstE(){return W_dstE;}
        uint64_t getW_valE(){return W_valE;}
        uint64_t getW_dstM(){return W_dstM;}
        uint64_t getW_valM(){return W_valM;}
        uint64_t getm_stat(){return m_stat;}
        uint64_t getW_stat(){return W_stat;}
        uint64_t getM_icode(){return M_icode;}
        bool getM_Cnd(){return M_Cnd;}
        uint64_t getM_valA(){return M_valA;}
        uint64_t getW_icode(){return W_icode;}
        void sete_dstE(uint64_t edstE){e_dstE = edstE;}
        void sete_valE(uint64_t evalE){e_valE = evalE;}
        void setM_dstM(uint64_t mdstM){M_dstM = mdstM;}
        void setm_valM(uint64_t mvalM){m_valM = mvalM;}
        void setM_dstE(uint64_t mdstE){M_dstE = mdstE;}
        void setM_valE(uint64_t mvalE){M_valE = mvalE;}
        void setW_dstE(uint64_t wdstE){W_dstE = wdstE;}
        void setW_valE(uint64_t wvalE){W_valE = wvalE;}
        void setW_dstM(uint64_t wdstM){W_dstM = wdstM;}
        void setW_valM(uint64_t wvalM){W_valM = wvalM;}
        void setm_stat(uint64_t mstat){m_stat = mstat;}
        void setW_stat(uint64_t Wstat){W_stat = Wstat;}
        void setM_icode(uint64_t Micode){M_icode = Micode;}
        void setM_Cnd(bool Mcnd){M_Cnd = Mcnd;}
        void setM_valA(uint64_t MvalA){M_valA = MvalA;}
        void setW_icode(uint64_t Wicode){W_icode = Wicode;}
};

#endif
