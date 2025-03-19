/**
 * This code creates an Antispam Hook to avoid payments based on your value amount criterias for XAH an IOUs.
   More info and how to use it: https://github.com/Ekiserrepe/antispam-hook
 */
#include "hookapi.h"

int64_t hook(uint32_t reserved ) {

    // Declare the keywords for the Hook Params
    uint8_t xah_param[3] = {'X', 'A', 'H'};
    uint8_t iou_param[3] = {'I', 'O', 'U'};
    uint8_t code_param[4] = {'C', 'O', 'D','E'};
    uint8_t issuer_param[6] = {'I', 'S', 'S','U','E','R'};

    //Namespaces
    //CODE
    uint8_t code_ns[32] = {0x07, 0xA9, 0xD7, 0xB4, 0xA9, 0xA2, 0x39, 0x15, 0xA6, 0x1B, 0xC8, 0x9B, 0xB0, 0x35, 0x7B, 0xF4, 0x7B, 0x34, 0x8C, 0xF4, 0x17, 0x4E, 0xB9, 0x65, 0xBB, 0x1D, 0xF8, 0xFB, 0xFA, 0x18, 0xB0, 0xB5};
    //CODEISSUER
    uint8_t codeissuer_ns[32] = {0x82, 0xC6, 0xE5, 0xE2, 0x88, 0xD0, 0xC9, 0x11, 0x60, 0x8D, 0xC8, 0x0D, 0x9F, 0x0B, 0xFF, 0x96, 0x9D, 0x49, 0x1B, 0x52, 0x4D, 0xC4, 0x86, 0x92, 0x65, 0x1B, 0xDC, 0x9B, 0xD1, 0x00, 0x01, 0x0B};
    //CODEISSUERCODE
    uint8_t codeissuercode_ns[32] = {0x68, 0x45, 0x8D, 0x41, 0xAF, 0x3D, 0x59, 0x37, 0xE3, 0x9A, 0xAC, 0xEB, 0xE1, 0x3A, 0xBE, 0x19, 0x9B, 0xD9, 0xBF, 0x27, 0x13, 0x6B, 0x77, 0x84, 0x86, 0xAB, 0x51, 0xA5, 0xAE, 0x0C, 0xC7, 0xD6};
    //CODEISSUERISSUER
    uint8_t codeissuerissuer_ns[32] = {0xFB, 0x09, 0xDA, 0x9F, 0x00, 0x38, 0x22, 0xC7, 0xD8, 0x9D, 0xB3, 0xEF, 0x0F, 0xD5, 0xE3, 0xF9, 0xA7, 0x43, 0x3F, 0x60, 0x97, 0x6F, 0xD7, 0xDC, 0x3C, 0xA5, 0xD5, 0x12, 0xF7, 0x51, 0x52, 0x2D};
    
    // Check hook account
    unsigned char hook_accid[20];
    hook_account((uint32_t)hook_accid, 20);

    // Check destination of the original txn
    uint8_t account_field[20];
    int32_t account_field_len = otxn_field(SBUF(account_field), sfDestination);

    // To know the type of origin txn
    int64_t tt = otxn_type();

    // Check if hook_accid and account_field are the same
    int equal = 0;
    BUFFER_EQUAL(equal, hook_accid, account_field, 20);

    //Fetch the sent Amount
    unsigned char amount_buffer[48];
    int64_t amount_len = otxn_field(SBUF(amount_buffer), sfAmount);
    int64_t drops_to_send = 0;

    //Getting XAH minimun amount accepted
    uint64_t xah_min_ns;
    state_foreign(SVAR(xah_min_ns), SBUF(xah_param), SBUF(code_ns), SBUF(hook_accid));
    otxn_param(SVAR(xah_min_ns), SBUF(xah_param));

    uint64_t xah_min_param;
    uint64_t iou_min_param;

    unsigned char currency_param_part[20]; 
    unsigned char issuer_param_part[20]; 
    unsigned char codeissuer_param_part[32];

    //Invoke to declare/update XAH min amount or delete it //64 LE
    if(!equal && tt==99 && otxn_param(SVAR(xah_min_param), SBUF(xah_param))>0)
    {
        if(xah_min_param>0){
            state_foreign_set(SVAR(xah_min_param), SBUF(xah_param), SBUF(code_ns), SBUF(hook_accid));
            accept(SBUF("Antispam: Added/Updated minimum value accepted for XAH"), 1);
        }
        else if(xah_min_param==0){
            state_foreign_set(0, 0, SBUF(xah_param), SBUF(code_ns), SBUF(hook_accid));
            accept(SBUF("Antispam: Deleting minimum value accepted for XAH"), 2);
        }    
    }

    //Invoke to IOU to declare/update minimun general amount or delete it//64 LE
    if(!equal && tt==99 && otxn_param(SVAR(iou_min_param), SBUF(iou_param))>0 && otxn_param(SVAR(iou_min_param), SBUF(iou_param))>0 && otxn_param(SBUF(currency_param_part), SBUF(code_param))!=20 && otxn_param(SBUF(issuer_param_part), SBUF(issuer_param))!=20)
    {
        if(iou_min_param>0){
            state_foreign_set(SVAR(iou_min_param), SBUF(iou_param), SBUF(code_ns), SBUF(hook_accid));
            accept(SBUF("Antispam: Added/Updated minimum value accepted for IOUs"), 3);
        }
        else if(iou_min_param==0){
            state_foreign_set(0, 0, SBUF(iou_param), SBUF(code_ns), SBUF(hook_accid));
            accept(SBUF("Antispam: Deleting minimum value accepted for IOUs"), 4);
        }    
    }
    otxn_param(SVAR(iou_min_param), SBUF(iou_param));
    otxn_param(SBUF(currency_param_part), SBUF(code_param));
    otxn_param(SBUF(issuer_param_part), SBUF(issuer_param));

    //Invoke to IOU Code+Issuer to declare/update minimum amount accepted or delete it //64 LE
    //codeissuer is a key based on code (20 bytes) + 12 first bytes from issuer. I'm not allowed by namespace to use 40 bytes.
    if(!equal && tt==99 && otxn_param(SVAR(iou_min_param), SBUF(iou_param))>0 && otxn_param(SBUF(currency_param_part), SBUF(code_param))==20 &&  otxn_param(SBUF(issuer_param_part), SBUF(issuer_param))==20)
    {
            for (int i = 0; GUARD(20), i < 20; i++) {
                codeissuer_param_part[i] = currency_param_part[i];  // Copia los primeros 20 bytes
            }
            for (int i = 0; GUARD(12), i < 12; i++) {
                codeissuer_param_part[i + 20] = issuer_param_part[i];  // Copia los segundos 20 bytes
            }
        if(iou_min_param>0){
            state_foreign_set(SVAR(iou_min_param), SBUF(codeissuer_param_part), SBUF(codeissuer_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(currency_param_part), SBUF(codeissuer_param_part), SBUF(codeissuercode_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(issuer_param_part), SBUF(codeissuer_param_part), SBUF(codeissuerissuer_ns), SBUF(hook_accid));

            accept(SBUF("Antispam: Added/Updated minimum value accepted for specific IOU"), 5);
        }
        else if(iou_min_param==0){
            state_foreign_set(0, 0, SBUF(codeissuer_param_part), SBUF(codeissuer_ns), SBUF(hook_accid));
            state_foreign_set(0, 0, SBUF(codeissuer_param_part), SBUF(codeissuercode_ns), SBUF(hook_accid));
            state_foreign_set(0, 0, SBUF(codeissuer_param_part), SBUF(codeissuerissuer_ns), SBUF(hook_accid));

            accept(SBUF("Antispam: Deleting minimum value accepted for specific IOU (Code Currency + Issuer)"), 6);
        }    
    }
    //Check if the incoming payment is XAH or IOU
    // No XAH (IOU)
    if (equal && tt==0 && amount_len != 8)
    {
        //IOU Amount comes with 8 bytes as xlf amount, 20 bytes currency and 20 bytes issuer
        // Variables to keep parts of the Amount
        unsigned char amount_part[8];    // First 8 bytes
        unsigned char currency_part[20];  // Next 20 bytes (code currency)
        unsigned char issuer_part[20];   // Last 20 bytes (issuer)
        unsigned char codeissuer_part[32];   // Key based on currency+issuer 20 bytes (currency) + 12 of the issuer


        for (int i = 0; GUARD(8), i < 8; ++i) {
            amount_part[i] = amount_buffer[i];
        }
        for (int i = 8; GUARD(20), i < 28; ++i) {
            currency_part[i - 8] = amount_buffer[i];
            //Generating codeissuer to check later if exists
            codeissuer_part[i - 8] = amount_buffer[i];
        }
        for (int i = 28; GUARD(20), i < 48; ++i) {
            issuer_part[i - 28] = amount_buffer[i];
        }
        for (int i = 28; GUARD(12), i < 40; i++) {
            // - 8 because you already filled 20 from code currency
                codeissuer_part[i - 8] = amount_buffer[i]; 
        }
        //Converting amount value
        int64_t amount_part_xfl = -INT64_FROM_BUF(amount_part);
        int64_t amount_drops_part = float_int(amount_part_xfl, 6, 1);

        int equal_code = 0;
        int equal_issuer = 0;

        unsigned char codeissuercode_from_ns[20];
        unsigned char codeissuerissuer_from_ns[20];

        uint64_t iou_general_min_ns;
        state_foreign(SVAR(iou_general_min_ns), SBUF(iou_param), SBUF(code_ns), SBUF(hook_accid));

        uint64_t iou_min_ns;

        state_foreign(SVAR(iou_min_ns), SBUF(codeissuer_part), SBUF(codeissuer_ns), SBUF(hook_accid));
        state_foreign(SVAR(codeissuercode_from_ns), SBUF(codeissuer_part), SBUF(codeissuercode_ns), SBUF(hook_accid));
        state_foreign(SVAR(codeissuerissuer_from_ns), SBUF(codeissuer_part), SBUF(codeissuerissuer_ns), SBUF(hook_accid));

        BUFFER_EQUAL(equal_code, codeissuercode_from_ns, currency_part, 20);
        BUFFER_EQUAL(equal_issuer, codeissuerissuer_from_ns, issuer_part, 20);

    if(equal_code && equal_issuer && amount_drops_part<iou_min_ns){
        rollback(SBUF("Antispam: This account declines this IOU transaction based on specific criteria"), 2);    
    }else if(!equal_code && !equal_issuer && state_foreign(SVAR(iou_general_min_ns), SBUF(iou_param), SBUF(code_ns), SBUF(hook_accid))>0 && amount_drops_part<iou_general_min_ns){
        rollback(SBUF("Antispam: This account declines this IOU transaction based on general criteria"), 2);    
    }else{        
        accept(SBUF("Antispam: IOU Payment accepted."), 7);
    }
}
    else if (equal && tt==0)// If it's XAH
    {
        int64_t otxn_drops = AMOUNT_TO_DROPS(amount_buffer);
        drops_to_send = (int64_t)((double)otxn_drops);

        if(otxn_drops<xah_min_ns){
            rollback(SBUF("Antispam: This account declines this transaction based on its minimum XAH value accepted."), 8);    
        }else{
            accept(SBUF("Antispam: This payment is accepted"), 9);
        }
    }
    accept (0,0,0); 
    return 0;
}