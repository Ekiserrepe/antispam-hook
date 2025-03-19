<p align="center">
  <img src="https://github.com/user-attachments/assets/76fb0ad9-c43f-4be9-b2a6-0e55789ca816">
</p>

# Anti-Spam Hook (A blocking hook example for Xahau) 

## Introduction

Anti-Spam Hook is a hook coded in C for the Xahau blockchain. A hook is a series of rules that enable smart logic in Xahau, considered the smart contracts for Xahau blockchain.

Anti-Spam Hook is a Hook that turns your Xahau account into a wizard to block unwanted incoming payments based on the minimum amount criteria and the type of currency used. This is an example hook and this problem can be approached in many other different ways.


## Explanation

**Attention:** Only use if you are sure of what you are doing on Testnet/Mainnet. You could put your funds at risk. It is recommended to install on new accounts.

This hook tries to solve the problem of spam generated in other similar networks such as XRP Ledger in which attackers send very small payments to send a message to the account owners.

The blocking offered by this hook is based on two criteria, currency and minimum amount accepted.

There are two basic groups to differentiate currencies, we will separate between XAH and IOUs.

The hook offers the possibility to use XAH and IOU as general blocking criteria with a minimum accepted amount.

This allows us to declare that we do not accept an amount less than 1 XAH and that we do not accept an amount less than 2 for IOUs, for example.

As IOUs have different supplies and a general criterion may not be enough, it is allowed to specify a specific IOU and the minimum accepted quantity. For this we will need the code currency of the IOU and its issuer and the minimum quantity accepted.

**Note:** If we have a general IOU criterion defined and several specific IOUs defined. The hook will ignore the general criteria defined for IOUs for the specific IOUs, being the individual criteria the only one that will affect them.

To define a general criterion we will use an Invoke transaction from the hook account. Using the Hook Param XAH and the value in drops minimum accepted. 1000000 drops = 1 XAH. 

If we want to create a general criterion for IOUs, we will create an Invoke transaction from the hook account. Using the Hook Param IOU and the value in minimum drops accepted. 1000000 drops = 1 IOU. 

If we want to add a specific IOU/Token we must create an Invoke transaction from the hook account. As Hook Param we will use IOU and the value in drops minimum accepted, the Hook Param CODE with the value in hexadecimal of the xrpCurrency and the Hook Param ISSUER with the address of the issuer in hexadecimal. The formats are specified below.

If we want to update any value of any of the previous options, we simply run another Invoke with the new drops number.

If we want to delete any of the registers, we only have to add as drops value “0000000000000000” and we will delete this register from our hook.

**Note:** Remember that if you delete a specific IOU rule, you will still be affected by the general IOU rule if you have it enabled for that token as well.



## How the data is stored on the Xahau blockchain

Xahau allows us to save information so that we can work with hooks and data we need. For the Anti-Spam hook we work with 4 namespaces or tables. These tables are generated in each account that installs the hook. 

The CODE namespace (table)

In this table we store the information for the general rules XAH and IOU. So we can have a maximum of two records, one with XAH as key and the minimum accepted value as value. And another with IOU as key and the minimum value accepted as value.

For example, if we were storing in table
- key: XAH | value: 1000000 drops
- key: IOU | value: 2000000 drops

it would look like this:

| key | value|
|-----|------|
|0000000000000000000000000000000000000000000000000000000000494F55 | 40420F0000000000 |
|0000000000000000000000000000000000000000000000000000000000494F55 | 80841E0000000000 |

For the IOU specific rules I had to create a special identifier that adds the code currency + the first 12 bytes of the IOU issuer. I have been forced to do this because I can't store it completely in the namespace. This problem could be handled in different ways, but I decided that this was the easiest way to have a simpler and easier to understand example.

We use 3 tables to store the information.

To explaining these tables we are going to use this example:

Rule to add:

Block EKI tokens payments below 1 EKI (1000000 drops EKI)

That information is:
- Minimum Amount accepted: 1000000
- Code Currency: EKI
- Issuer: EFD294519933A9D37EA262DF81FAFCCF3B009EE6

Translated to namespace/hooks language:

- Minimum Amount accepted: 40420F0000000000
- Code Currency: 000000000000000000000000454B490000000000
- Issuer: EFD294519933A9D37EA262DF81FAFCCF3B009EE6

The new ID created by Code Currency + 12 bytes of the Issuer: 000000000000000000000000454B490000000000EFD294519933A9D37EA262DF

CODEISSUER namespace, which will store the new code+issuer identifier and the minimum quantity accepted.

| key | value|
|-----|------|
|000000000000000000000000454B490000000000EFD294519933A9D37EA262DF | 40420F0000000000 |

CODEISSUERCODE namespace, where we will store the new code+issuer as key and the codecurrency as value.

| key | value|
|-----|------|
|000000000000000000000000454B490000000000EFD294519933A9D37EA262DF | 000000000000000000000000454B490000000000 |

CODEISSUERISSUER namespace, where we will store the new code+issuer as key and the complete issuer address as value.

| key | value|
|-----|------|
|000000000000000000000000454B490000000000EFD294519933A9D37EA262DF | EFD294519933A9D37EA262DF81FAFCCF3B009EE6 |

Note: As this example is didactic I have saved the information in this way to make it clearer. The hook could have been programmed in another way to need less tables.


## Installation & Usage

Once the hook is installed, the following triggers are expected for the hook.

- An account will send a payment of XAH to the hook account. If the amount of the payment is less than the XAH rule of thumb recorded in the hook, it will be rejected. If there is no general rule or if the minimum value accepted is less than the amount received, it will be accepted.

- A Invoke transaction from the hook account with "XAH" Hook Param and a minimum value in drops as value will create/update/delete a general rule for XAH payments.

- A Invoke transaction from the hook account with "IOU" Hook Param and a minimum value in drops as value will create/update/delete a general rule for IOU payments. (This rule will not affect specific IOU payments)

- A Invoke transaction from the hook account with "IOU" Hook Param and a minimum value in drops as value, "CODE" with hex value of the currency name in xrpCurrency format, "ISSUER" with hex value of the address of the issuer, will create/update/delete a specific rule for that IOU payments.


## How to install the Highest Number Hook on Testnet?

HookHash: CE8477A94BE00A9EEDA46FB519617FCE3D119914AEAEFAD15D9E434323B112AB

1. You can do it by [XRPLWin Hook Install Tool](https://xahau-testnet.xrplwin.com/tools/hook/from-hash)

2. Or you can do it sending the transaction below:

HookOn is activated to trigger for Invoke and Payment transactions. You can verify it copying the HookOn value (FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFFFFFFFFFBFFFFE) in this website: https://richardah.github.io/xrpl-hookon-calculator/

    const prepared = {
      "TransactionType": "SetHook",
      "Account": your_account_address,
      "Flags": 0,
      "Hooks": [
        {
          "Hook": {
            "HookHash": "CE8477A94BE00A9EEDA46FB519617FCE3D119914AEAEFAD15D9E434323B112AB",
            "HookNamespace": "0000000000000000000000000000000000000000000000000000000000000000",
            "HookOn": "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFFFFFFFFFBFFFFE",
          }
        }
      ],
      ...networkInfo.txValues,
    };

## How to install the Highest Number Hook on JSHooks Testnet?

Work in progress...

## How to uninstall the Highest Number Hook on Mainnet/Testnet?

    const prepared = {
      "TransactionType": "SetHook",
      "Account": your_account_address,
      "Flags": 0,
      Hooks:
    [        
        {                        
            Hook: {
                CreateCode: "",
                Flags: 1,
            }
        }
    ],
      ...networkInfo.txValues,
    };

## Transaction Examples for Hook Parameters

## Creating/Updating a new general XAH rule using XAH Hook Param

We create a Invoke transaction with our Hook Account as "Account" and NO Destination Account. 

Hook Parameters and values will be:
- XAH
- Minimum value accepted in drops
-------
- XAH
- int number as uint64 LE



In this example we want to add 1000000 drops as XAH (1 XAH) minimum value accepted that is 40420F0000000000 translated as uint64 LE. You can visit https://transia-rnd.github.io/xrpl-hex-visualizer/ , Insert the value 1000000 and click on To Hex button, you will see that the value will be the same as we added. XAH param string is HEX translated: 584148. (For https://builder.xahau.network/ IDE you don't need to translate Parameter Name)


    const prepared = {
      TransactionType: "Invoke",
      Account: your_account_address,
      Flags: 0,
      HookParameters: [
        {
      "HookParameter": {
        "HookParameterName": "584148",
        "HookParameterValue": "40420F0000000000"
      }
    }
    ],
      ...networkInfo.txValues,
    };


## Creating/Updating a new general IOU rule using IOU Hook Param

We create a Invoke transaction with our Hook Account as "Account" and NO Destination Account. 

Hook Parameters and values will be:
- IOU
- Minimum value accepted in drops
-------
- IOU
- int number as uint64 LE



In this example we want to add 1000000 drops as IOU (1 IOU) minimum value accepted that is 40420F0000000000 translated as uint64 LE. You can visit https://transia-rnd.github.io/xrpl-hex-visualizer/ , Insert the value 1000000 and click on To Hex button, you will see that the value will be the same as we added. IOU param string is HEX translated: 494F55. (For https://builder.xahau.network/ IDE you don't need to translate Parameter Name)


    const prepared = {
      TransactionType: "Invoke",
      Account: your_account_address,
      Flags: 0,
      HookParameters: [
        {
      "HookParameter": {
        "HookParameterName": "494F55",
        "HookParameterValue": "40420F0000000000"
      }
    }
    ],
      ...networkInfo.txValues,
    };


## Creating/Updating a specific IOU rule using IOU, CODE, ISSUER Hook Params

We create a Invoke transaction with our Hook Account as "Account" and NO Destination Account. 

Hook Parameters and values will be:
- IOU
- Minimum value accepted in drops
- CODE
- CodeCurrency
- ISSUER
- Issuer Address
-------
- IOU
- int number as uint64 LE
- CODE
- xrpCurrency HEX
- ISSUER
- Issuer Address
- xrpAddress HEX



In this example we want to add 1000000 drops for EKI token (1 EKI) minimum value accepted that is 40420F0000000000 translated as uint64 LE. You can visit https://transia-rnd.github.io/xrpl-hex-visualizer/ , Insert the value 1000000 and click on To Hex button, you will see that the value will be the same as we added. IOU param string is HEX translated: 494F55. (For https://builder.xahau.network/ IDE you don't need to translate Parameter Name). For the value of the CODE parameter, you need the token name in CodeCurrency format. In this case we take EKI and click “To Hex” and copy xrpCurrency: 000000000000000000000000454B490000000000 (https://transia-rnd.github.io/xrpl-hex-visualizer/) For the value of the ISSUER parameter, you need the issuer address in xrpAddress format. In this case we take r41hmwUZRTuMtdNDjueteTQg8xqAXbq8Kz and click “To Hex” and copy xrpCurrency: EFD294519933A9D37EA262DF81FAFCCF3B009EE6


    const prepared = {
      TransactionType: "Invoke",
      Account: your_account_address,
      Flags: 0,
      HookParameters": [
    {
      "HookParameter": {
        "HookParameterName": "494F55",
        "HookParameterValue": "40420F0000000000"
      }
    },
    {
      "HookParameter": {
        "HookParameterName": "434F4445",
        "HookParameterValue": "000000000000000000000000454B490000000000"
      }
    },
    {
      "HookParameter": {
        "HookParameterName": "495353554552",
        "HookParameterValue": "EFD294519933A9D37EA262DF81FAFCCF3B009EE6"
      }
    }
  ],
      ...networkInfo.txValues,
    };

## Deleting any rule

To delete any rule at the minimum accepted value you must specify: 0000000000000000

For example:


    const prepared = {
      TransactionType: "Invoke",
      Account: your_account_address,
      Flags: 0,
      HookParameters: [
        {
      "HookParameter": {
        "HookParameterName": "584148",
        "HookParameterValue": "0000000000000000"
      }
    }
    ],
      ...networkInfo.txValues,
    };



## How to install the Hook on Mainnet?

Same as Testnet but changing the hookhash. The Hookhash is CE8477A94BE00A9EEDA46FB519617FCE3D119914AEAEFAD15D9E434323B112AB.

1. You can do it by [XRPLWin Hook Install Tool](https://xahau.xrplwin.com/tools/hook/from-hash)

2. Or you can do it sending the transaction below:

```
    const prepared = {
      "TransactionType": "SetHook",
      "Account": your_account_address,
      "Flags": 0,
      "Hooks": [
        {
          "Hook": {
            "HookHash": "CE8477A94BE00A9EEDA46FB519617FCE3D119914AEAEFAD15D9E434323B112AB",
            "HookNamespace": "0000000000000000000000000000000000000000000000000000000000000000",
            "HookOn": "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFFFFFFFFFBFFFFE",
          }
        }
      ],
      ...networkInfo.txValues,
    };
```

## Ideas for your hooks

This is just a small example so that others can build similar ideas or extend the current hook. Some ideas I propose:

1. Add that any payment of a general or specific rule is always blocked, with no minimum value accepted.
2. Add the option to block transactions with memo.
3. Add the option to add a maximum accepted value.

## Special thanks

Thanks to [@denis_angell](https://x.com/angell_denis) , [@Satish_nl](https://x.com/satish_nl) and [@TeQu](https://x.com/_tequ_) for being there when i get stuck.
