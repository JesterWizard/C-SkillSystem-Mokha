#include "common-chax.h"
#include "bwl.h"
#include "../../../Contants/Texts/build/msgs.h"

const struct SupportTalkEnt gNewSupportTalkList[] = 
{
    { CHARACTER_EIRIKA,   CHARACTER_EPHRAIM,  {MSG_0C53, MSG_0C54, MSG_0C55} },
    { CHARACTER_EIRIKA,   CHARACTER_SETH,     {MSG_0C56, MSG_0C57, MSG_0C58} },
    { CHARACTER_EIRIKA,   CHARACTER_SALEH,    {MSG_0C65, MSG_0C66, MSG_0C67} },
    { CHARACTER_EIRIKA,   CHARACTER_LARACHEL, {MSG_0C59, MSG_0C5A, MSG_0C5B} },
    { CHARACTER_EIRIKA,   CHARACTER_TANA,     {MSG_0C5C, MSG_0C5D, MSG_0C5E} },
    { CHARACTER_EIRIKA,   CHARACTER_INNES,    {MSG_0C5F, MSG_0C60, MSG_0C61} },
    { CHARACTER_EIRIKA,   CHARACTER_FORDE,    {MSG_0C62, MSG_0C63, MSG_0C64} },

    { CHARACTER_SETH,     CHARACTER_FRANZ,    {MSG_0C7A, MSG_0C7B, MSG_0C7C} },
    { CHARACTER_SETH,     CHARACTER_EIRIKA,   {MSG_0C56, MSG_0C57, MSG_0C58} },
    { CHARACTER_SETH,     CHARACTER_GARCIA,   {MSG_0C7D, MSG_0C7E, MSG_0C7F} },
    { CHARACTER_SETH,     CHARACTER_NATASHA,  {MSG_0C80, MSG_0C81, MSG_0C82} },
    { CHARACTER_SETH,     CHARACTER_CORMAG,   {MSG_0C83, MSG_0C84, MSG_0C85} },
    { CHARACTER_SETH,     CHARACTER_DUESSEL,  {C_Support_Seth_Duessel, B_Support_Seth_Duessel, A_Support_Seth_Duessel} },

    { CHARACTER_GILLIAM,  CHARACTER_GARCIA,   {MSG_0C92, MSG_0C93, MSG_0C94} },
    { CHARACTER_GILLIAM,  CHARACTER_FRANZ,    {MSG_0C86, MSG_0C87, MSG_0C88} },
    { CHARACTER_GILLIAM,  CHARACTER_SYRENE,   {MSG_0C95, MSG_0C96, MSG_0C97} },
    { CHARACTER_GILLIAM,  CHARACTER_MOULDER,  {MSG_0C98, MSG_0C99, MSG_0C9A} },
    { CHARACTER_GILLIAM,  CHARACTER_NEIMI,    {MSG_0C9B, MSG_0C9C, MSG_0C9D} },

    { CHARACTER_FRANZ,    CHARACTER_SETH,     {MSG_0C7A, MSG_0C7B, MSG_0C7C} },
    { CHARACTER_FRANZ,    CHARACTER_GILLIAM,  {MSG_0C86, MSG_0C87, MSG_0C88} },
    { CHARACTER_FRANZ,    CHARACTER_FORDE,    {MSG_0C8C, MSG_0C8D, MSG_0C8E} },
    { CHARACTER_FRANZ,    CHARACTER_AMELIA,   {MSG_0C8F, MSG_0C90, MSG_0C91} },
    { CHARACTER_FRANZ,    CHARACTER_NATASHA,  {MSG_0C89, MSG_0C8A, MSG_0C8B} },

    { CHARACTER_MOULDER,  CHARACTER_VANESSA,  {MSG_0C9E, MSG_0C9F, MSG_0CA0} },
    { CHARACTER_MOULDER,  CHARACTER_COLM,     {MSG_0CA1, MSG_0CA2, MSG_0CA3} },
    { CHARACTER_MOULDER,  CHARACTER_GILLIAM,  {MSG_0C98, MSG_0C99, MSG_0C9A} },
    { CHARACTER_MOULDER,  CHARACTER_SYRENE,   {MSG_0CA4, MSG_0CA5, MSG_0CA6} },

    { CHARACTER_VANESSA,  CHARACTER_MOULDER,  {MSG_0C9E, MSG_0C9F, MSG_0CA0} },
    { CHARACTER_VANESSA,  CHARACTER_SYRENE,   {MSG_0CA7, MSG_0CA8, MSG_0CA9} },
    { CHARACTER_VANESSA,  CHARACTER_LUTE,     {MSG_0CAA, MSG_0CAB, MSG_0CAC} },
    { CHARACTER_VANESSA,  CHARACTER_FORDE,    {MSG_0CAD, MSG_0CAE, MSG_0CAF} },
    { CHARACTER_VANESSA,  CHARACTER_INNES,    {MSG_0CB0, MSG_0CB1, MSG_0CB2} },

    { CHARACTER_ROSS,     CHARACTER_GARCIA,   {MSG_0CB3, MSG_0CB4, MSG_0CB5} },
    { CHARACTER_ROSS,     CHARACTER_LUTE,     {MSG_0CBC, MSG_0CBD, MSG_0CBE} },
    { CHARACTER_ROSS,     CHARACTER_AMELIA,   {MSG_0CBF, MSG_0CC0, MSG_0CC1} },
    { CHARACTER_ROSS,     CHARACTER_EWAN,     {MSG_0CC2, MSG_0CC3, MSG_0CC4} },
    { CHARACTER_ROSS,     CHARACTER_GERIK,    {MSG_0CC5, MSG_0CC6, MSG_0CC7} },

    { CHARACTER_NEIMI,    CHARACTER_COLM,     {MSG_0CC8, MSG_0CC9, MSG_0CCA} },
    { CHARACTER_NEIMI,    CHARACTER_GARCIA,   {MSG_0CB9, MSG_0CBA, MSG_0CBB} },
    { CHARACTER_NEIMI,    CHARACTER_AMELIA,   {MSG_0CD4, MSG_0CD5, MSG_0CD6} },
    { CHARACTER_NEIMI,    CHARACTER_ARTUR,    {MSG_0CD7, MSG_0CD8, MSG_0CD9} },
    { CHARACTER_NEIMI,    CHARACTER_GILLIAM,  {MSG_0C9B, MSG_0C9C, MSG_0C9D} },

    { CHARACTER_COLM,     CHARACTER_NEIMI,    {MSG_0CC8, MSG_0CC9, MSG_0CCA} },
    { CHARACTER_COLM,     CHARACTER_MARISA,   {MSG_0CCB, MSG_0CCC, MSG_0CCD} },
    { CHARACTER_COLM,     CHARACTER_RENNAC,   {MSG_0CCE, MSG_0CCF, MSG_0CD0} },
    { CHARACTER_COLM,     CHARACTER_MOULDER,  {MSG_0CA1, MSG_0CA2, MSG_0CA3} },
    { CHARACTER_COLM,     CHARACTER_KYLE,     {MSG_0CD1, MSG_0CD2, MSG_0CD3} },

    { CHARACTER_GARCIA,   CHARACTER_ROSS,     {MSG_0CB3, MSG_0CB4, MSG_0CB5} },
    { CHARACTER_GARCIA,   CHARACTER_DOZLA,    {MSG_0CB6, MSG_0CB7, MSG_0CB8} },
    { CHARACTER_GARCIA,   CHARACTER_SETH,     {MSG_0C7D, MSG_0C7E, MSG_0C7F} },
    { CHARACTER_GARCIA,   CHARACTER_NEIMI,    {MSG_0CB9, MSG_0CBA, MSG_0CBB} },
    { CHARACTER_GARCIA,   CHARACTER_GILLIAM,  {MSG_0C92, MSG_0C93, MSG_0C94} },

    { CHARACTER_INNES,    CHARACTER_GERIK,    {MSG_0D13, MSG_0D14, MSG_0D15} },
    { CHARACTER_INNES,    CHARACTER_VANESSA,  {MSG_0CB0, MSG_0CB1, MSG_0CB2} },
    { CHARACTER_INNES,    CHARACTER_TANA,     {MSG_0D0D, MSG_0D0E, MSG_0D0F} },
    { CHARACTER_INNES,    CHARACTER_JOSHUA,   {MSG_0CF5, MSG_0CF6, MSG_0CF7} },
    { CHARACTER_INNES,    CHARACTER_LARACHEL, {MSG_0D16, MSG_0D17, MSG_0D18} },
    { CHARACTER_INNES,    CHARACTER_EIRIKA,   {MSG_0C5F, MSG_0C60, MSG_0C61} },

    { CHARACTER_LUTE,     CHARACTER_ARTUR,    {MSG_0CDA, MSG_0CDB, MSG_0CDC} },
    { CHARACTER_LUTE,     CHARACTER_ROSS,     {MSG_0CBC, MSG_0CBD, MSG_0CBE} },
    { CHARACTER_LUTE,     CHARACTER_KNOLL,    {C_Support_Knoll_Lute, B_Support_Knoll_Lute, A_Support_Knoll_Lute} },
    { CHARACTER_LUTE,     CHARACTER_KYLE,     {MSG_0CE9, MSG_0CEA, MSG_0CEB} },
    { CHARACTER_LUTE,     CHARACTER_VANESSA,  {MSG_0CAA, MSG_0CAB, MSG_0CAC} },

    { CHARACTER_NATASHA,  CHARACTER_JOSHUA,   {MSG_0CEC, MSG_0CED, MSG_0CEE} },
    { CHARACTER_NATASHA,  CHARACTER_KNOLL,    {MSG_0CEF, MSG_0CF0, MSG_0CF1} },
    { CHARACTER_NATASHA,  CHARACTER_CORMAG,   {MSG_0CF2, MSG_0CF3, MSG_0CF4} },
    { CHARACTER_NATASHA,  CHARACTER_SETH,     {MSG_0C80, MSG_0C81, MSG_0C82} },
    { CHARACTER_NATASHA,  CHARACTER_FRANZ,    {MSG_0C89, MSG_0C8A, MSG_0C8B} },

    { CHARACTER_CORMAG,   CHARACTER_DUESSEL,  {MSG_0D43, MSG_0D44, MSG_0D45} },
    { CHARACTER_CORMAG,   CHARACTER_TANA,     {MSG_0D0A, MSG_0D0B, MSG_0D0C} },
    { CHARACTER_CORMAG,   CHARACTER_NATASHA,  {MSG_0CF2, MSG_0CF3, MSG_0CF4} },
    { CHARACTER_CORMAG,   CHARACTER_ARTUR,    {MSG_0CE0, MSG_0CE1, MSG_0CE2} },
    { CHARACTER_CORMAG,   CHARACTER_SETH,     {MSG_0C83, MSG_0C84, MSG_0C85} },

    { CHARACTER_EPHRAIM,  CHARACTER_EIRIKA,   {MSG_0C53, MSG_0C54, MSG_0C55} },
    { CHARACTER_EPHRAIM,  CHARACTER_KYLE,     {MSG_0C68, MSG_0C69, MSG_0C6A} },
    { CHARACTER_EPHRAIM,  CHARACTER_FORDE,    {MSG_0C6B, MSG_0C6C, MSG_0C6D} },
    { CHARACTER_EPHRAIM,  CHARACTER_MYRRH,    {MSG_0C6E, MSG_0C6F, MSG_0C70} },
    { CHARACTER_EPHRAIM,  CHARACTER_LARACHEL, {MSG_0C71, MSG_0C72, MSG_0C73} },
    { CHARACTER_EPHRAIM,  CHARACTER_DUESSEL,  {MSG_0C74, MSG_0C75, MSG_0C76} },
    { CHARACTER_EPHRAIM,  CHARACTER_TANA,     {MSG_0C77, MSG_0C78, MSG_0C79} },

    { CHARACTER_FORDE,    CHARACTER_KYLE,     {MSG_0D01, MSG_0D02, MSG_0D03} },
    { CHARACTER_FORDE,    CHARACTER_VANESSA,  {MSG_0CAD, MSG_0CAE, MSG_0CAF} },
    { CHARACTER_FORDE,    CHARACTER_EPHRAIM,  {MSG_0C6B, MSG_0C6C, MSG_0C6D} },
    { CHARACTER_FORDE,    CHARACTER_FRANZ,    {MSG_0C8C, MSG_0C8D, MSG_0C8E} },
    { CHARACTER_FORDE,    CHARACTER_EIRIKA,   {MSG_0C62, MSG_0C63, MSG_0C64} },

    { CHARACTER_KYLE,     CHARACTER_FORDE,    {MSG_0D01, MSG_0D02, MSG_0D03} },
    { CHARACTER_KYLE,     CHARACTER_EPHRAIM,  {MSG_0C68, MSG_0C69, MSG_0C6A} },
    { CHARACTER_KYLE,     CHARACTER_SYRENE,   {MSG_0D04, MSG_0D05, MSG_0D06} },
    { CHARACTER_KYLE,     CHARACTER_COLM,     {MSG_0CD1, MSG_0CD2, MSG_0CD3} },
    { CHARACTER_KYLE,     CHARACTER_LUTE,     {MSG_0CE9, MSG_0CEA, MSG_0CEB} },

    { CHARACTER_AMELIA,   CHARACTER_NEIMI,    {MSG_0CD4, MSG_0CD5, MSG_0CD6} },
    { CHARACTER_AMELIA,   CHARACTER_DUESSEL,  {MSG_0D46, MSG_0D47, MSG_0D48} },
    { CHARACTER_AMELIA,   CHARACTER_EWAN,     {MSG_0D34, MSG_0D35, MSG_0D36} },
    { CHARACTER_AMELIA,   CHARACTER_ROSS,     {MSG_0CBF, MSG_0CC0, MSG_0CC1} },
    { CHARACTER_AMELIA,   CHARACTER_FRANZ,    {MSG_0C8F, MSG_0C90, MSG_0C91} },

    { CHARACTER_ARTUR,    CHARACTER_LUTE,     {MSG_0CDA, MSG_0CDB, MSG_0CDC} },
    { CHARACTER_ARTUR,    CHARACTER_TETHYS,   {MSG_0CDD, MSG_0CDE, MSG_0CDF} },
    { CHARACTER_ARTUR,    CHARACTER_CORMAG,   {MSG_0CE0, MSG_0CE1, MSG_0CE2} },
    { CHARACTER_ARTUR,    CHARACTER_NEIMI,    {MSG_0CD7, MSG_0CD8, MSG_0CD9} },
    { CHARACTER_ARTUR,    CHARACTER_JOSHUA,   {MSG_0CE3, MSG_0CE4, MSG_0CE5} },

    { CHARACTER_GERIK,    CHARACTER_INNES,    {MSG_0D13, MSG_0D14, MSG_0D15} },
    { CHARACTER_GERIK,    CHARACTER_TETHYS,   {MSG_0D19, MSG_0D1A, MSG_0D1B} },
    { CHARACTER_GERIK,    CHARACTER_MARISA,   {MSG_0D1C, MSG_0D1D, MSG_0D1E} },
    { CHARACTER_GERIK,    CHARACTER_SALEH,    {MSG_0D1F, MSG_0D20, MSG_0D21} },
    { CHARACTER_GERIK,    CHARACTER_JOSHUA,   {MSG_0CFE, MSG_0CFF, MSG_0D00} },
    { CHARACTER_GERIK,    CHARACTER_ROSS,     {MSG_0CC5, MSG_0CC6, MSG_0CC7} },

    { CHARACTER_TETHYS,   CHARACTER_EWAN,     {MSG_0D22, MSG_0D23, MSG_0D24} },
    { CHARACTER_TETHYS,   CHARACTER_GERIK,    {MSG_0D19, MSG_0D1A, MSG_0D1B} },
    { CHARACTER_TETHYS,   CHARACTER_MARISA,   {MSG_0D25, MSG_0D26, MSG_0D27} },
    { CHARACTER_TETHYS,   CHARACTER_RENNAC,   {MSG_0D28, MSG_0D29, MSG_0D2A} },
    { CHARACTER_TETHYS,   CHARACTER_ARTUR,    {MSG_0CDD, MSG_0CDE, MSG_0CDF} },

    { CHARACTER_MARISA,   CHARACTER_TANA,     {MSG_0D10, MSG_0D11, MSG_0D12} },
    { CHARACTER_MARISA,   CHARACTER_TETHYS,   {MSG_0D25, MSG_0D26, MSG_0D27} },
    { CHARACTER_MARISA,   CHARACTER_GERIK,    {MSG_0D1C, MSG_0D1D, MSG_0D1E} },
    { CHARACTER_MARISA,   CHARACTER_COLM,     {MSG_0CCB, MSG_0CCC, MSG_0CCD} },
    { CHARACTER_MARISA,   CHARACTER_JOSHUA,   {MSG_0CFB, MSG_0CFC, MSG_0CFD} },

    { CHARACTER_SALEH,    CHARACTER_MYRRH,    {MSG_0D2B, MSG_0D2C, MSG_0D2D} },
    { CHARACTER_SALEH,    CHARACTER_EWAN,     {MSG_0D2E, MSG_0D2F, MSG_0D30} },
    { CHARACTER_SALEH,    CHARACTER_GERIK,    {MSG_0D1F, MSG_0D20, MSG_0D21} },
    { CHARACTER_SALEH,    CHARACTER_EIRIKA,   {MSG_0C65, MSG_0C66, MSG_0C67} },

    { CHARACTER_EWAN,     CHARACTER_TETHYS,   {MSG_0D22, MSG_0D23, MSG_0D24} },
    { CHARACTER_EWAN,     CHARACTER_SALEH,    {MSG_0D2E, MSG_0D2F, MSG_0D30} },
    { CHARACTER_EWAN,     CHARACTER_DOZLA,    {MSG_0D31, MSG_0D32, MSG_0D33} },
    { CHARACTER_EWAN,     CHARACTER_AMELIA,   {MSG_0D34, MSG_0D35, MSG_0D36} },
    { CHARACTER_EWAN,     CHARACTER_ROSS,     {MSG_0CC2, MSG_0CC3, MSG_0CC4} },
    { CHARACTER_EWAN,     CHARACTER_MYRRH,    {C_Support_Ewan_Myrrh, B_Support_Ewan_Myrrh, A_Support_Ewan_Myrrh} },

    { CHARACTER_LARACHEL, CHARACTER_DOZLA,    {MSG_0D37, MSG_0D38, MSG_0D39} },
    { CHARACTER_LARACHEL, CHARACTER_RENNAC,   {MSG_0D3A, MSG_0D3B, MSG_0D3C} },
    { CHARACTER_LARACHEL, CHARACTER_JOSHUA,   {MSG_0CF8, MSG_0CF9, MSG_0CFA} },
    { CHARACTER_LARACHEL, CHARACTER_EIRIKA,   {MSG_0C59, MSG_0C5A, MSG_0C5B} },
    { CHARACTER_LARACHEL, CHARACTER_EPHRAIM,  {MSG_0C71, MSG_0C72, MSG_0C73} },
    { CHARACTER_LARACHEL, CHARACTER_INNES,    {MSG_0D16, MSG_0D17, MSG_0D18} },

    { CHARACTER_DOZLA,    CHARACTER_LARACHEL, {MSG_0D37, MSG_0D38, MSG_0D39} },
    { CHARACTER_DOZLA,    CHARACTER_MYRRH,    {MSG_0D3D, MSG_0D3E, MSG_0D3F} },
    { CHARACTER_DOZLA,    CHARACTER_RENNAC,   {MSG_0D40, MSG_0D41, MSG_0D42} },
    { CHARACTER_DOZLA,    CHARACTER_EWAN,     {MSG_0D31, MSG_0D32, MSG_0D33} },
    { CHARACTER_DOZLA,    CHARACTER_GARCIA,   {MSG_0CB6, MSG_0CB7, MSG_0CB8} },

    { CHARACTER_RENNAC,   CHARACTER_TETHYS,   {MSG_0D28, MSG_0D29, MSG_0D2A} },
    { CHARACTER_RENNAC,   CHARACTER_LARACHEL, {MSG_0D3A, MSG_0D3B, MSG_0D3C} },
    { CHARACTER_RENNAC,   CHARACTER_DOZLA,    {MSG_0D40, MSG_0D41, MSG_0D42} },
    { CHARACTER_RENNAC,   CHARACTER_COLM,     {MSG_0CCE, MSG_0CCF, MSG_0CD0} },
    { CHARACTER_RENNAC,   CHARACTER_JOSHUA,   {C_Support_Joshua_Rennac, B_Support_Joshua_Rennac, A_Support_Joshua_Rennac} },

    { CHARACTER_DUESSEL,  CHARACTER_CORMAG,   {MSG_0D43, MSG_0D44, MSG_0D45} },
    { CHARACTER_DUESSEL,  CHARACTER_AMELIA,   {MSG_0D46, MSG_0D47, MSG_0D48} },
    { CHARACTER_DUESSEL,  CHARACTER_KNOLL,    {MSG_0D49, MSG_0D4A, MSG_0D4B} },
    { CHARACTER_DUESSEL,  CHARACTER_EPHRAIM,  {MSG_0C74, MSG_0C75, MSG_0C76} },
    { CHARACTER_DUESSEL,  CHARACTER_SETH,     {C_Support_Seth_Duessel, B_Support_Seth_Duessel, A_Support_Seth_Duessel} },

    { CHARACTER_MYRRH,    CHARACTER_SALEH,    {MSG_0D2B, MSG_0D2C, MSG_0D2D} },
    { CHARACTER_MYRRH,    CHARACTER_DOZLA,    {MSG_0D3D, MSG_0D3E, MSG_0D3F} },
    { CHARACTER_MYRRH,    CHARACTER_EPHRAIM,  {MSG_0C6E, MSG_0C6F, MSG_0C70} },
    { CHARACTER_MYRRH,    CHARACTER_EWAN,     {C_Support_Ewan_Myrrh, B_Support_Ewan_Myrrh, A_Support_Ewan_Myrrh} },

    { CHARACTER_KNOLL,    CHARACTER_NATASHA,  {MSG_0CEF, MSG_0CF0, MSG_0CF1} },
    { CHARACTER_KNOLL,    CHARACTER_DUESSEL,  {MSG_0D49, MSG_0D4A, MSG_0D4B} },
    { CHARACTER_KNOLL,    CHARACTER_LUTE,     {C_Support_Knoll_Lute, B_Support_Knoll_Lute, A_Support_Knoll_Lute} },

    { CHARACTER_JOSHUA,   CHARACTER_NATASHA,  {MSG_0CEC, MSG_0CED, MSG_0CEE} },
    { CHARACTER_JOSHUA,   CHARACTER_INNES,    {MSG_0CF5, MSG_0CF6, MSG_0CF7} },
    { CHARACTER_JOSHUA,   CHARACTER_ARTUR,    {MSG_0CE3, MSG_0CE4, MSG_0CE5} },
    { CHARACTER_JOSHUA,   CHARACTER_LARACHEL, {MSG_0CF8, MSG_0CF9, MSG_0CFA} },
    { CHARACTER_JOSHUA,   CHARACTER_MARISA,   {MSG_0CFB, MSG_0CFC, MSG_0CFD} },
    { CHARACTER_JOSHUA,   CHARACTER_GERIK,    {MSG_0CFE, MSG_0CFF, MSG_0D00} },
    { CHARACTER_JOSHUA,   CHARACTER_RENNAC,   {C_Support_Joshua_Rennac, B_Support_Joshua_Rennac, A_Support_Joshua_Rennac} },

    { CHARACTER_SYRENE,   CHARACTER_KYLE,     {MSG_0D04, MSG_0D05, MSG_0D06} },
    { CHARACTER_SYRENE,   CHARACTER_VANESSA,  {MSG_0CA7, MSG_0CA8, MSG_0CA9} },
    { CHARACTER_SYRENE,   CHARACTER_MOULDER,  {MSG_0CA4, MSG_0CA5, MSG_0CA6} },
    { CHARACTER_SYRENE,   CHARACTER_GILLIAM,  {MSG_0C95, MSG_0C96, MSG_0C97} },
    { CHARACTER_SYRENE,   CHARACTER_TANA,     {MSG_0D07, MSG_0D08, MSG_0D09} },

    { CHARACTER_TANA,     CHARACTER_EPHRAIM,  {MSG_0C77, MSG_0C78, MSG_0C79} },
    { CHARACTER_TANA,     CHARACTER_SYRENE,   {MSG_0D07, MSG_0D08, MSG_0D09} },
    { CHARACTER_TANA,     CHARACTER_CORMAG,   {MSG_0D0A, MSG_0D0B, MSG_0D0C} },
    { CHARACTER_TANA,     CHARACTER_INNES,    {MSG_0D0D, MSG_0D0E, MSG_0D0F} },
    { CHARACTER_TANA,     CHARACTER_EIRIKA,   {MSG_0C5C, MSG_0C5D, MSG_0C5E} },
    { CHARACTER_TANA,     CHARACTER_MARISA,   {MSG_0D10, MSG_0D11, MSG_0D12} },

    { 0xFFFF,             0x0000,             {0x0000, 0x000, 0x0000} }
};