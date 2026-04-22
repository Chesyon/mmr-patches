#include <pmdsky.h>
#include <cot.h>
#include "extern.h"
#include "memory.h"

#define UNBEKANNT 99

int reihenfolge[12];
int istBekannt[12];//2er Paare, 0u1, 2u3 etc. Für Bot
int paarAufgebraucht[6];//Check ob Kombis weg sind, 0 für 0u1, 1 für 2u3 etc. Für Bot und Spieler

void randomisation(){// + variablen initialisierung
    int max = 12;
    int counter;
    for (int i = 0; i < max; i++) {
        reihenfolge[i] = i;
    }
    
    for (int i = max - 1; i > 0; i--) {
    int j = RandRange(0, i + 1);
    int temp = reihenfolge[i];
    reihenfolge[i] = reihenfolge[j];
    reihenfolge[j] = temp;
    }

    for (int i = 0; i < 6; i++) {
    paarAufgebraucht[i] = 0;//init 
    }

    for (int i = 0; i < 12; i++) {
    istBekannt[i] = 0;//init
    }

    for (int i = 0; i < 25; i++) {
    counter = 200 + i;
    SaveScriptVariableValueAtIndex(NULL, VAR_SCENARIO_TALK_BIT_FLAG, counter, 0);//init
    }
    
}

int getPositionOfActor(int actorId) {
    for (int i = 0; i < 12; i++) {
        if (reihenfolge[i] == actorId) {
            return i;
        }
    }
    return -1;
}



int getCardPosition (int offset) {
    int pos = getPositionOfActor(offset);
    switch (pos){ 
        case 0:
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0, 8192);
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0, 11264);
            break;
        case 1:
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0, 24576);
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0, 11264);
            break;
        case 2:
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0, 40960);
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0, 11264);
            break;
        case 3:
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0, 57344);
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0, 11264);
            break;
        case 4:
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0, 8192);
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0, 25600);
            break;
        case 5:
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0, 24576);
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0, 25600);
            break;
        case 6:
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0, 40960);
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0, 25600);
            break;
        case 7:
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0, 57344);
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0, 25600);
            break;
        case 8:
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0, 8192);
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0, 39936);
            break;
        case 9:
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0, 24576);
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0, 39936);
            break;
        case 10:
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0, 40960);
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0, 39936);
            break;
        case 11:
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0, 57344);
            SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0, 39936);
            break;
        default:
        return 0;
    }
    return 0;
}

int checkTouchScreenPress(){
    switch(TSPressed){
        case 0:
        return 0;
        case 1:
        return 1;
    }
    return 128;
}

int checkSelectedCard(){
    int TSX = TSXPosLive;
    int TSY = TSYPosLive;
    int kartenID;
    //Oben Links
    if (TSX < 48 && TSX > 16 && TSY < 64 && TSY > 16){
        kartenID = reihenfolge[0]/2;
        if (paarAufgebraucht[kartenID] == 1){
            return 0;
        }
        return 1;
    }
    //Oben Mitte Links
    else if (TSX < 112 && TSX > 80 && TSY < 64 && TSY > 16){
        kartenID = reihenfolge[1]/2;
        if (paarAufgebraucht[kartenID] == 1){
            return 0;
        }
        return 2;
    }
    //Oben Mitte Rechts
    else if (TSX < 176 && TSX > 144 && TSY < 64 && TSY > 16){
        kartenID = reihenfolge[2]/2;
        if (paarAufgebraucht[kartenID] == 1){
            return 0;
        }
        return 3;
    }
    //Oben Recht
    else if (TSX < 240 && TSX > 208 && TSY < 64 && TSY > 16){
        kartenID = reihenfolge[3]/2;
        if (paarAufgebraucht[kartenID] == 1){
            return 0;
        }
        return 4;
    }
    //Mitte Links
    else if (TSX < 48 && TSX > 16 && TSY < 120 && TSY > 72){
        kartenID = reihenfolge[4]/2;
        if (paarAufgebraucht[kartenID] == 1){
            return 0;
        }
        return 5;
    }
    //Mitte Mitte Links
    else if (TSX < 112 && TSX > 80 && TSY < 120 && TSY > 72){
        kartenID = reihenfolge[5]/2;
        if (paarAufgebraucht[kartenID] == 1){
            return 0;
        }
        return 6;
    }
    //Mitte Mitte Rechts
    else if (TSX < 176 && TSX > 144 && TSY < 120 && TSY > 72){
        kartenID = reihenfolge[6]/2;
        if (paarAufgebraucht[kartenID] == 1){
            return 0;
        }
        return  7;
    }
    //Mitte Rechts
    else if (TSX < 240 && TSX > 208 && TSY < 120 && TSY > 72){
        kartenID = reihenfolge[7]/2;
        if (paarAufgebraucht[kartenID] == 1){
            return 0;
        }
        return 8;
    }
    //Unten Links
    else if (TSX < 48 && TSX > 16 && TSY < 176 && TSY > 128){
        kartenID = reihenfolge[8]/2;
        if (paarAufgebraucht[kartenID] == 1){
            return 0;
        }
        return 9;
    }
    //Unten Mitte Links
    else if (TSX < 112 && TSX > 80 && TSY < 176 && TSY > 128){
        kartenID = reihenfolge[9]/2;
        if (paarAufgebraucht[kartenID] == 1){
            return 0;
        }
        return 10;
    }
    //Unten Mitte Rechts
    else if (TSX < 176 && TSX > 144 && TSY < 176 && TSY > 128){
        kartenID = reihenfolge[10]/2;
        if (paarAufgebraucht[kartenID] == 1){
            return 0;
        }
        return 11;
    }
    //Unten Rechts
    else if (TSX < 240 && TSX > 208 && TSY < 176 && TSY > 128){
        kartenID = reihenfolge[11]/2;
        if (paarAufgebraucht[kartenID] == 1){
            return 0;
        }
        return 12;
    }
    return 128;
}

int checkPaar() {//ziehen werte aus talk flags ausm Script
    int karteEins = 0;
    int karteZwei = 0;
    int temp = 0;
    int counter = 0;
    for (int i = 0; i < 12; i++) {
    counter = 200 + i;
    temp = LoadScriptVariableValueAtIndex(NULL, VAR_SCENARIO_TALK_BIT_FLAG, counter);
    if (temp == 1) {
            if (karteEins == 0) {
                karteEins = i + 1;
            } else {
                karteZwei = i + 1;
                break; 
            }
        }
    }
    for (int i = 0; i < 12; i++) {
    counter = 200 + i;
    SaveScriptVariableValueAtIndex(NULL, VAR_SCENARIO_TALK_BIT_FLAG, counter, 0);
    }
    //KartenauswahlsID für WerteID
    int wert1 = reihenfolge[karteEins - 1];
    int wert2 = reihenfolge[karteZwei - 1];

    SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 1, wert1);
    SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 2, wert2);
    int id1 = (reihenfolge[karteEins-1]) / 2;
    int id2 = (reihenfolge[karteZwei-1]) / 2;
    if (id1 == id2) {
        if (id1 >= 0 && id1 <= 5) {//Check ob ID valide
            paarAufgebraucht[id1] = 1;
            return id1;
        }
        return 192; //Ungültige ID, für den Fall der Fälle
    } else {
        return 128; //Kein Paar
    }
        
}

int checkEnde() {
    int gefundenePaare = 0;
    for (int i = 0; i < 6; i++) {
        if (paarAufgebraucht[i] == 1) {
            gefundenePaare++;
        }
    }
    
    if (gefundenePaare == 6) {
        return 1;//Ende
    }
    return 0;
}

void fuellBekanntArray() {
    for (int i = 0; i < 12; i++) {
        int bekannt = LoadScriptVariableValueAtIndex(NULL, VAR_SCENARIO_TALK_BIT_FLAG, 212 + i);
        if (bekannt == 1) {
            istBekannt[i] = reihenfolge[i]; 
        } else {
            istBekannt[i] = UNBEKANNT; // läuft mind. einmal vor bot turn, sonst alles tot
        }
    }
}

void getKartenWerte(){
    int karteEins = 0;
    int karteZwei = 0;
    int temp = 0;
    int counter = 0;
    for (int i = 0; i < 12; i++) {
    counter = 200 + i;
    temp = LoadScriptVariableValueAtIndex(NULL, VAR_SCENARIO_TALK_BIT_FLAG, counter);
    if (temp == 1) {
            if (karteEins == 0) {
                karteEins = i + 1;
            } else {
                karteZwei = i + 1;
                break; 
            }
        }
    }
    int wert1 = reihenfolge[karteEins - 1];
    int wert2 = reihenfolge[karteZwei - 1];
    SaveScriptVariableValueAtIndex(NULL, VAR_CRYSTAL_COLOR_01, 0, wert1);
    SaveScriptVariableValueAtIndex(NULL, VAR_CRYSTAL_COLOR_02, 0, wert2);
}

void getKarteEinsWert(){//Nur für Player relevant, fair play und so
    int karteEins = 0;
    int temp = 0;
    int counter = 0;
    for (int i = 0; i < 12; i++) {
    counter = 200 + i;
    temp = LoadScriptVariableValueAtIndex(NULL, VAR_SCENARIO_TALK_BIT_FLAG, counter);
    if (temp == 1) {
            karteEins = i + 1;
        }
    }
    int wert1 = reihenfolge[karteEins - 1];
    SaveScriptVariableValueAtIndex(NULL, VAR_CRYSTAL_COLOR_02, 0, wert1);
}

void botZug() {//eugh
    int wahl1 = UNBEKANNT;
    int wahl2 = UNBEKANNT;

    int verfuegbareIndizes[12];
    int anzahlVerfuegbar = 0;
    for (int i = 0; i < 12; i++) {
        int kartenWert = reihenfolge[i];
        int paarId = kartenWert / 2;
        if (paarAufgebraucht[paarId] == 0) {
            verfuegbareIndizes[anzahlVerfuegbar++] = i;
        }
    }

    for (int i = 0; i < anzahlVerfuegbar; i++) {
        int idxA = verfuegbareIndizes[i];
        for (int j = i + 1; j < anzahlVerfuegbar; j++) {
            int idxB = verfuegbareIndizes[j];
            if (istBekannt[idxA] != UNBEKANNT && istBekannt[idxB] != UNBEKANNT) {
                if ((istBekannt[idxA] / 2) == (istBekannt[idxB] / 2)) {
                    wahl1 = idxA;
                    wahl2 = idxB;
                    goto zug_ausfuehren;
                }
            }
        }
    }

    int r1 = RandRange(0, anzahlVerfuegbar); 
    wahl1 = verfuegbareIndizes[r1];
    for (int i = 0; i < anzahlVerfuegbar; i++) {
        int idxPruef = verfuegbareIndizes[i];
        if (idxPruef != wahl1 && istBekannt[idxPruef] != UNBEKANNT) {
            if ((reihenfolge[wahl1] / 2) == (istBekannt[idxPruef] / 2)) {
                wahl2 = idxPruef;
                goto zug_ausfuehren;
            }
        }
    }

    int r2 = RandRange(0, anzahlVerfuegbar - 1);
    if (r2 >= r1) {
        wahl2 = verfuegbareIndizes[r2 + 1];
    } else {
        wahl2 = verfuegbareIndizes[r2];
    }

zug_ausfuehren:
    //bekannt array aktualisieren
    istBekannt[wahl1] = reihenfolge[wahl1];
    istBekannt[wahl2] = reihenfolge[wahl2];
    //Redundanz für die aktualisierung über game flags
    SaveScriptVariableValueAtIndex(NULL, VAR_SCENARIO_TALK_BIT_FLAG, 212 + wahl1, 1);
    SaveScriptVariableValueAtIndex(NULL, VAR_SCENARIO_TALK_BIT_FLAG, 212 + wahl2, 1);

    SaveScriptVariableValueAtIndex(NULL, VAR_SCENARIO_TALK_BIT_FLAG, 200 + wahl1, 1);
    SaveScriptVariableValueAtIndex(NULL, VAR_SCENARIO_TALK_BIT_FLAG, 200 + wahl2, 1);
}