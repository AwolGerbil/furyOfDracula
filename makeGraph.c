#include "HunterView.h"
#include "cities.h"

static void makeGraph (HunterView hv){
    /* ROAD Connections*/

    // ALICANTE
    addLink(hv, ALICANTE, SARAGOSSA, LAND);
    addLink(hv, ALICANTE, GRANADA, LAND);
    addLink(hv, ALICANTE, MADRID, LAND);

    // AMSTERDAM
    addLink(hv, AMSTERDAM, COLOGNE, LAND);
    addLink(hv, AMSTERDAM, BRUSSELS, LAND);

    // ATHENS
    addLink(hv, ATHENS, VALONA, LAND);

    // BARCELONA
    addLink(hv, BARCELONA, SARAGOSSA, LAND);
    addLink(hv, BARCELONA, TOULOUSE, LAND);

    // BARI
    addLink(hv, BARI, NAPLES, LAND);
    addLink(hv, BARI, ROME, LAND);

    // BELGRADE
    addLink(hv, BELGRADE, SARAJEVO, LAND);
    addLink(hv, BELGRADE, ST_JOSEPH_AND_ST_MARYS, LAND);
    addLink(hv, BELGRADE, SZEGED, LAND);
    addLink(hv, BELGRADE, KLAUSENBURG, LAND);
    addLink(hv, BELGRADE, BUCHAREST, LAND);
    addLink(hv, BELGRADE, SOFIA, LAND);

    // BERLIN
    addLink(hv, BERLIN, LEIPZIG, LAND);
    addLink(hv, BERLIN, HAMBURG, LAND);
    addLink(hv, BERLIN, PRAGUE, LAND);

    // BORDEAUX
    addLink(hv, BORDEAUX, NANTES, LAND);
    addLink(hv, BORDEAUX, CLERMONT_FERRAND, LAND);
    addLink(hv, BORDEAUX, TOULOUSE, LAND);
    addLink(hv, BORDEAUX, SARAGOSSA, LAND);

    // BRUSSELS
    addLink(hv, BRUSSELS, AMSTERDAM, LAND);
    addLink(hv, BRUSSELS, COLOGNE, LAND);
    addLink(hv, BRUSSELS, STRASBOURG, LAND);
    addLink(hv, BRUSSELS, PARIS, LAND);
    addLink(hv, BRUSSELS, LE_HAVRE, LAND);

    // BUCHAREST
    addLink(hv, BUCHAREST, GALATZ, LAND);
    addLink(hv, BUCHAREST, CONSTANTA, LAND);
    addLink(hv, BUCHAREST, SOFIA, LAND);
    addLink(hv, BUCHAREST, BELGRADE, LAND);
    addLink(hv, BUCHAREST, KLAUSENBURG, LAND);

    // BUDAPEST
    addLink(hv, BUDAPEST, ZAGREB, LAND);
    addLink(hv, BUDAPEST, VIENNA, LAND);
    addLink(hv, BUDAPEST,KLAUSENBURG, LAND);
    addLink(hv, BUDAPEST, SZEGED, LAND);

    // CADIZ
    addLink(hv, CADIZ, GRANADA, LAND);
    addLink(hv, CADIZ, MADRID, LAND);
    addLink(hv, CADIZ, LISBON, LAND);

    // CASTLE_DRACULA
    addLink(hv, CASTLE_DRACULA, GALATZ, LAND);
    addLink(hv, CASTLE_DRACULA, KLAUSENBURG, LAND);

    // CLERMONT_FERRAND
    addLink(hv, CLERMONT_FERRAND, MARSEILLES, LAND);
    addLink(hv, CLERMONT_FERRAND, GENEVA, LAND);
    addLink(hv, CLERMONT_FERRAND, TOULOUSE, LAND);
    addLink(hv, CLERMONT_FERRAND, PARIS, LAND);
    addLink(hv, CLERMONT_FERRAND, BORDEAUX, LAND);
    addLink(hv, CLERMONT_FERRAND, NANTES, LAND);

    // COLOGNE
    addLink(hv, COLOGNE, STRASBOURG, LAND);
    addLink(hv, COLOGNE, BRUSSELS, LAND);
    addLink(hv, COLOGNE, FRANKFURT, LAND);
    addLink(hv, COLOGNE, HAMBURG, LAND);
    addLink(hv, COLOGNE, AMSTERDAM, LAND);
    addLink(hv, COLOGNE, LEIPZIG, LAND);

    // CONSTANTA
    addLink(hv, CONSTANTA, VARNA, LAND);
    addLink(hv, CONSTANTA, BUCHAREST, LAND);
    addLink(hv, CONSTANTA, GALATZ, LAND);

    // DUBLIN
    addLink(hv, DUBLIN, GALWAY, LAND);

    // EDINBURGH
    addLink(hv, EDINBURGH, MANCHESTER, LAND);

    // FLORENCE
    addLink(hv, FLORENCE, GENOA, LAND);
    addLink(hv, FLORENCE, VENICE, LAND);
    addLink(hv, FLORENCE, ROME, LAND);

    // FRANKFURT
    addLink(hv, FRANKFURT, STRASBOURG, LAND);
    addLink(hv, FRANKFURT, COLOGNE, LAND);
    addLink(hv, FRANKFURT, LEIPZIG, LAND);
    addLink(hv, FRANKFURT, NUREMBURG, LAND);

    // GALATZ
    addLink(hv, GALATZ, CONSTANTA, LAND);
    addLink(hv, GALATZ, BUCHAREST, LAND);
    addLink(hv, GALATZ, KLAUSENBURG, LAND);
    addLink(hv, GALATZ, CASTLE_DRACULA, LAND);

    // GALWAY
    addLink(hv, GALWAY, DUBLIN, LAND);

    // GENEVA
    addLink(hv, GENEVA, ZURICH, LAND);
    addLink(hv, GENEVA, STRASBOURG, LAND);
    addLink(hv, GENEVA, PARIS, LAND);
    addLink(hv, GENEVA, CLERMONT_FERRAND, LAND);
    addLink(hv, GENEVA, MARSEILLES, LAND);

    // GENOA
    addLink(hv, GENOA, VENICE, LAND);
    addLink(hv, GENOA, FLORENCE, LAND);
    addLink(hv, GENOA, MILAN, LAND);
    addLink(hv, GENOA, MARSEILLES, LAND);

    // GRANADA
    addLink(hv, GRANADA, ALICANTE, LAND);
    addLink(hv, GRANADA, MADRID, LAND);
    addLink(hv, GRANADA, CADIZ, LAND);

    // HAMBURG
    addLink(hv, HAMBURG, COLOGNE, LAND);
    addLink(hv, HAMBURG, BERLIN, LAND);
    addLink(hv, HAMBURG, LEIPZIG, LAND);

    // ST_JOSEPH_AND_ST_MARYS
    addLink(hv, ST_JOSEPH_AND_ST_MARYS, SARAJEVO, LAND);
    addLink(hv, ST_JOSEPH_AND_ST_MARYS, ZAGREB, LAND);
    addLink(hv, ST_JOSEPH_AND_ST_MARYS, SZEGED, LAND);
    addLink(hv, ST_JOSEPH_AND_ST_MARYS, BELGRADE, LAND);

    // KLAUSENBURG
    addLink(hv, KLAUSENBURG, BELGRADE, LAND);
    addLink(hv, KLAUSENBURG, SZEGED, LAND);
    addLink(hv, KLAUSENBURG, BUDAPEST, LAND);
    addLink(hv, KLAUSENBURG, CASTLE_DRACULA, LAND);
    addLink(hv, KLAUSENBURG, GALATZ, LAND);
    addLink(hv, KLAUSENBURG, BUCHAREST, LAND);

    // LE_HAVRE
    addLink(hv, LE_HAVRE, PARIS, LAND);
    addLink(hv, LE_HAVRE, BRUSSELS, LAND);
    addLink(hv, LE_HAVRE, NANTES, LAND);

    // LEIPZIG
    addLink(hv, LEIPZIG, FRANKFURT, LAND);
    addLink(hv, LEIPZIG, COLOGNE, LAND);
    addLink(hv, LEIPZIG, HAMBURG, LAND);
    addLink(hv, LEIPZIG, BERLIN, LAND);
    addLink(hv, LEIPZIG, NUREMBURG, LAND);

    // LISBON
    addLink(hv, LISBON, CADIZ, LAND);
    addLink(hv, LISBON, MADRID, LAND);
    addLink(hv, LISBON, SANTANDER, LAND);

    // LIVERPOOL
    addLink(hv, LIVERPOOL, SWANSEA, LAND);
    addLink(hv, LIVERPOOL, EDINBURGH, LAND);

    // LONDON
    addLink(hv, LONDON, PLYMOUTH, LAND);
    addLink(hv, LONDON, SWANSEA, LAND);
    addLink(hv, LONDON, MANCHESTER, LAND);

    // MADRID
    addLink(hv, MADRID, LISBON, LAND);
    addLink(hv, MADRID, CADIZ, LAND);
    addLink(hv, MADRID, GRANADA, LAND);
    addLink(hv, MADRID, ALICANTE, LAND);
    addLink(hv, MADRID, SANTANDER, LAND);
    addLink(hv, MADRID, SARAGOSSA, LAND);

    // MANCHESTER
    addLink(hv, MANCHESTER, LIVERPOOL, LAND);
    addLink(hv, MANCHESTER, EDINBURGH, LAND);
    addLink(hv, MANCHESTER, LONDON, LAND);

    // MARSEILLES
    addLink(hv, MARSEILLES, GENEVA, LAND);
    addLink(hv, MARSEILLES, ZURICH, LAND);
    addLink(hv, MARSEILLES, MILAN, LAND);
    addLink(hv, MARSEILLES, GENOA, LAND);
    addLink(hv, MARSEILLES, CLERMONT_FERRAND, LAND);
    addLink(hv, MARSEILLES, TOULOUSE, LAND);

    // MILAN
    addLink(hv, MILAN, GENOA, LAND);
    addLink(hv, MILAN, VENICE, LAND);
    addLink(hv, MILAN, MARSEILLES, LAND);
    addLink(hv, MILAN, ZURICH, LAND);
    addLink(hv, MILAN, MUNICH, LAND);

    // MUNICH
    addLink(hv, MUNICH, MILAN, LAND);
    addLink(hv, MUNICH, ZURICH, LAND);
    addLink(hv, MUNICH, STRASBOURG, LAND);
    addLink(hv, MUNICH, NUREMBURG, LAND);
    addLink(hv, MUNICH, VIENNA, LAND);
    addLink(hv, MUNICH, ZAGREB, LAND);
    addLink(hv, MUNICH, VENICE, LAND);

    // NANTES
    addLink(hv, NANTES, LE_HAVRE, LAND);
    addLink(hv, NANTES, PARIS, LAND);
    addLink(hv, NANTES, BORDEAUX, LAND);
    addLink(hv, NANTES, CLERMONT_FERRAND, LAND);

    // NAPLES
    addLink(hv, NAPLES, ROME, LAND);
    addLink(hv, NAPLES, BARI, LAND);

    // NUREMBURG
    addLink(hv, NUREMBURG, STRASBOURG, LAND);
    addLink(hv, NUREMBURG, FRANKFURT, LAND);
    addLink(hv, NUREMBURG, LEIPZIG, LAND);
    addLink(hv, NUREMBURG, PRAGUE, LAND);
    addLink(hv, NUREMBURG, MUNICH, LAND);

    // PARIS
    addLink(hv, PARIS, STRASBOURG, LAND);
    addLink(hv, PARIS, GENEVA, LAND);
    addLink(hv, PARIS, BRUSSELS, LAND);
    addLink(hv, PARIS, LE_HAVRE, LAND);
    addLink(hv, PARIS, NANTES, LAND);
    addLink(hv, PARIS, CLERMONT_FERRAND, LAND);

    // PLYMOUTH
    addLink(hv, PLYMOUTH, LONDON, LAND);

    // PRAGUE
    addLink(hv, PRAGUE, NUREMBURG, LAND);
    addLink(hv, PRAGUE, BERLIN, LAND);
    addLink(hv, PRAGUE, VIENNA, LAND);

    // ROME
    addLink(hv, ROME, FLORENCE, LAND);
    addLink(hv, ROME, NAPLES, LAND);
    addLink(hv, ROME, BARI, LAND);

    // SALONICA
    addLink(hv, SALONICA, VALONA, LAND);
    addLink(hv, SALONICA, SOFIA, LAND);

    // SANTANDER
    addLink(hv, SANTANDER, MADRID, LAND);
    addLink(hv, SANTANDER, LISBON, LAND);
    addLink(hv, SANTANDER, SARAGOSSA, LAND);

    // SARAGOSSA
    addLink(hv, SARAGOSSA, TOULOUSE, LAND);
    addLink(hv, SARAGOSSA, BARCELONA, LAND);
    addLink(hv, SARAGOSSA, BORDEAUX, LAND);
    addLink(hv, SARAGOSSA, ALICANTE, LAND);
    addLink(hv, SARAGOSSA, MADRID, LAND);
    addLink(hv, SARAGOSSA, SANTANDER, LAND);

    // SARAJEVO
    addLink(hv, SARAJEVO, VALONA, LAND);
    addLink(hv, SARAJEVO, ZAGREB, LAND);
    addLink(hv, SARAJEVO, ST_JOSEPH_AND_ST_MARYS, LAND);
    addLink(hv, SARAJEVO, BELGRADE, LAND);
    addLink(hv, SARAJEVO, SOFIA, LAND);

    // SOFIA
    addLink(hv, SOFIA, VARNA, LAND);
    addLink(hv, SOFIA, SALONICA, LAND);
    addLink(hv, SOFIA, VALONA, LAND);
    addLink(hv, SOFIA, SARAJEVO, LAND);
    addLink(hv, SOFIA, BELGRADE, LAND);
    addLink(hv, SOFIA, BUCHAREST, LAND);

    // STRASBOURG
    addLink(hv, STRASBOURG, ZURICH, LAND);
    addLink(hv, STRASBOURG, MUNICH, LAND);
    addLink(hv, STRASBOURG, GENEVA, LAND);
    addLink(hv, STRASBOURG, NUREMBURG, LAND);
    addLink(hv, STRASBOURG, FRANKFURT, LAND);
    addLink(hv, STRASBOURG, COLOGNE, LAND);
    addLink(hv, STRASBOURG, BRUSSELS, LAND);
    addLink(hv, STRASBOURG, PARIS, LAND);

    // SWANSEA
    addLink(hv, SWANSEA, LONDON, LAND);
    addLink(hv, SWANSEA, LIVERPOOL, LAND);

    // SZEGED
    addLink(hv, SZEGED, ST_JOSEPH_AND_ST_MARYS, LAND);
    addLink(hv, SZEGED, ZAGREB, LAND);
    addLink(hv, SZEGED, BUDAPEST, LAND);
    addLink(hv, SZEGED, KLAUSENBURG, LAND);
    addLink(hv, SZEGED, BELGRADE, LAND);

    // TOULOUSE
    addLink(hv, TOULOUSE, BORDEAUX, LAND);
    addLink(hv, TOULOUSE, CLERMONT_FERRAND, LAND);
    addLink(hv, TOULOUSE, MARSEILLES, LAND);
    addLink(hv, TOULOUSE, BARCELONA, LAND);
    addLink(hv, TOULOUSE, SARAGOSSA, LAND);

    // VALONA
    addLink(hv, VALONA, ATHENS, LAND);
    addLink(hv, VALONA, SALONICA, LAND);
    addLink(hv, VALONA, SOFIA, LAND);
    addLink(hv, VALONA, SARAJEVO, LAND);

    // VARNA
    addLink(hv, VARNA, SOFIA, LAND);
    addLink(hv, VARNA, CONSTANTA, LAND);

    // VENICE
    addLink(hv, VENICE, FLORENCE, LAND);
    addLink(hv, VENICE, GENOA, LAND);
    addLink(hv, VENICE, MILAN, LAND);
    addLink(hv, VENICE, MUNICH, LAND);

    // VIENNA
    addLink(hv, VIENNA, ZAGREB, LAND);
    addLink(hv, VIENNA, MUNICH, LAND);
    addLink(hv, VIENNA, PRAGUE, LAND);
    addLink(hv, VIENNA, BUDAPEST, LAND);

    // ZAGREB
    addLink(hv, ZAGREB, SARAJEVO, LAND);
    addLink(hv, ZAGREB, MUNICH, LAND);
    addLink(hv, ZAGREB, VIENNA, LAND);
    addLink(hv, ZAGREB, BUDAPEST, LAND);
    addLink(hv, ZAGREB, SZEGED, LAND);
    addLink(hv, ZAGREB, ST_JOSEPH_AND_ST_MARYS, LAND);

    // ZURICH
    addLink(hv, ZURICH, MILAN, LAND);
    addLink(hv, ZURICH, MUNICH, LAND);
    addLink(hv, ZURICH, GENEVA, LAND);
    addLink(hv, ZURICH, MARSEILLES, LAND);
    addLink(hv, ZURICH, STRASBOURG, LAND);

    /* RAIL Connections */

    // ALICANTE
    addLink(hv, ALICANTE, MADRID, RAIL);
    addLink(hv, ALICANTE, BARCELONA, RAIL);

    // BARCELONA
    addLink(hv, BARCELONA, ALICANTE, RAIL);
    addLink(hv, BARCELONA, SARAGOSSA, RAIL);

    // BARI
    addLink(hv, BARI, NAPLES, RAIL);

    // BELGRADE
    addLink(hv, BELGRADE, SZEGED, RAIL);
    addLink(hv, BELGRADE, SOFIA, RAIL);

    // BERLIN
    addLink(hv, BERLIN, HAMBURG, RAIL);
    addLink(hv, BERLIN, PRAGUE, RAIL);
    addLink(hv, BERLIN, LEIPZIG, RAIL);

    // BORDEAUX
    addLink(hv, BORDEAUX, SARAGOSSA, RAIL);
    addLink(hv, BORDEAUX, PARIS, RAIL);

    // BRUSSELS
    addLink(hv, BRUSSELS, COLOGNE, RAIL);
    addLink(hv, BRUSSELS, PARIS, RAIL);

    // BUCHAREST
    addLink(hv, BUCHAREST, SZEGED, RAIL);
    addLink(hv, BUCHAREST, GALATZ, RAIL);
    addLink(hv, BUCHAREST, CONSTANTA, RAIL);

    // BUDAPEST
    addLink(hv, BUDAPEST, VIENNA, RAIL);
    addLink(hv, BUDAPEST, SZEGED, RAIL);

    // COLOGNE
    addLink(hv, COLOGNE, BRUSSELS, RAIL);
    addLink(hv, COLOGNE, FRANKFURT, RAIL);

    // CONSTANTA
    addLink(hv, CONSTANTA, BUCHAREST, RAIL);

    // FLORENCE
    addLink(hv, FLORENCE, ROME, RAIL);
    addLink(hv, FLORENCE, MILAN, RAIL);

    // FRANKFURT
    addLink(hv, FRANKFURT, STRASBOURG, RAIL);
    addLink(hv, FRANKFURT, LEIPZIG, RAIL);
    addLink(hv, FRANKFURT, COLOGNE, RAIL);

    // GALATZ
    addLink(hv, GALATZ, BUCHAREST, RAIL);

    // HAMBURG
    addLink(hv, HAMBURG, BERLIN, RAIL);

    // LEIPZIG
    addLink(hv, LEIPZIG, FRANKFURT, RAIL);
    addLink(hv, LEIPZIG, NUREMBURG, RAIL);
    addLink(hv, LEIPZIG, BERLIN, RAIL);

    // LISBON
    addLink(hv, LISBON, MADRID, RAIL);

    // LIVERPOOL
    addLink(hv, LIVERPOOL, MANCHESTER, RAIL);

    // LONDON
    addLink(hv, LONDON, SWANSEA, RAIL);
    addLink(hv, LONDON, MANCHESTER, RAIL);

    // MADRID
    addLink(hv, MADRID, ALICANTE, RAIL);
    addLink(hv, MADRID, LISBON, RAIL);
    addLink(hv, MADRID, SANTANDER, RAIL);
    addLink(hv, MADRID, SARAGOSSA, RAIL);

    // MILAN
    addLink(hv, MILAN, GENEVA, RAIL);
    addLink(hv, MILAN, FLORENCE, RAIL);
    addLink(hv, MILAN, ZURICH, RAIL);
    addLink(hv, MILAN, GENOA, RAIL);

    // MANCHESTER
    addLink(hv, MANCHESTER, LONDON, RAIL);
    addLink(hv, MANCHESTER, LIVERPOOL, RAIL);
    addLink(hv, MANCHESTER, EDINBURGH, RAIL);

    // NAPLES
    addLink(hv, NAPLES, ROME, RAIL);
    addLink(hv, NAPLES, BARI, RAIL);

    // NUREMBURG
    addLink(hv, NUREMBURG, MUNICH, RAIL);
    addLink(hv, NUREMBURG, LEIPZIG, RAIL);

    // PARIS
    addLink(hv, PARIS, BORDEAUX, RAIL);
    addLink(hv, PARIS, MARSEILLES, RAIL);
    addLink(hv, PARIS, LE_HAVRE, RAIL);
    addLink(hv, PARIS, BRUSSELS, RAIL);

    // PRAGUE
    addLink(hv, PRAGUE, BERLIN, RAIL);
    addLink(hv, PRAGUE, VIENNA, RAIL);

    // ROME
    addLink(hv, ROME, FLORENCE, RAIL);
    addLink(hv, ROME, NAPLES, RAIL);

    // SALONICA
    addLink(hv, SALONICA, SOFIA, RAIL);

    // SANTANDER
    addLink(hv, SANTANDER, MADRID, RAIL);

    // SARAGOSSA
    addLink(hv, SARAGOSSA, MADRID, RAIL);
    addLink(hv, SARAGOSSA, BARCELONA, RAIL);
    addLink(hv, SARAGOSSA, BORDEAUX, RAIL);

    // SOFIA
    addLink(hv, SOFIA, BELGRADE, RAIL);
    addLink(hv, SOFIA, SALONICA, RAIL);
    addLink(hv, SOFIA, VARNA, RAIL);

    // STRASBOURG
    addLink(hv, STRASBOURG, FRANKFURT, RAIL);
    addLink(hv, STRASBOURG, ZURICH, RAIL);

    // SWANSEA
    addLink(hv, SWANSEA, LONDON, RAIL);

    // SZEGED
    addLink(hv, SZEGED, BUDAPEST, RAIL);
    addLink(hv, SZEGED, BELGRADE, RAIL);
    addLink(hv, SZEGED, BUCHAREST, RAIL);

    // VARNA
    addLink(hv, VARNA, SOFIA, RAIL);

    // VENICE
    addLink(hv, VENICE, VIENNA, RAIL);

    // VIENNA
    addLink(hv, VIENNA, PRAGUE, RAIL);
    addLink(hv, VIENNA, VENICE, RAIL);
    addLink(hv, VIENNA, BUDAPEST, RAIL);

    // ZURICH
    addLink(hv, ZURICH, MILAN, RAIL);
    addLink(hv, ZURICH, STRASBOURG, RAIL);

    /* SEA Connections */


    // NORTH_SEA
    addLink(hv, NORTH_SEA, ATLANTIC_OCEAN, SEA);
    addLink(hv, NORTH_SEA, ENGLISH_CHANNEL, SEA);
    addLink(hv, NORTH_SEA, EDINBURGH, SEA);
    addLink(hv, NORTH_SEA, AMSTERDAM, SEA);
    addLink(hv, NORTH_SEA, HAMBURG, SEA);

    // ENGLISH_CHANNEL
    addLink(hv, ENGLISH_CHANNEL, ATLANTIC_OCEAN, SEA);
    addLink(hv, ENGLISH_CHANNEL, NORTH_SEA, SEA);
    addLink(hv, ENGLISH_CHANNEL, LE_HAVRE, SEA);
    addLink(hv, ENGLISH_CHANNEL, PLYMOUTH, SEA);
    addLink(hv, ENGLISH_CHANNEL, LONDON, SEA);

    // IRISH_SEA
    addLink(hv, IRISH_SEA, ATLANTIC_OCEAN, SEA);
    addLink(hv, IRISH_SEA, LIVERPOOL, SEA);
    addLink(hv, IRISH_SEA, SWANSEA, SEA);
    addLink(hv, IRISH_SEA, DUBLIN, SEA);

    // ATLANTIC_OCEAN
    addLink(hv, ATLANTIC_OCEAN, NORTH_SEA, SEA);
    addLink(hv, ATLANTIC_OCEAN, BAY_OF_BISCAY, SEA);
    addLink(hv, ATLANTIC_OCEAN, IRISH_SEA, SEA);
    addLink(hv, ATLANTIC_OCEAN, ENGLISH_CHANNEL, SEA);
    addLink(hv, ATLANTIC_OCEAN, MEDITERRANEAN_SEA, SEA);
    addLink(hv, ATLANTIC_OCEAN, CADIZ, SEA);
    addLink(hv, ATLANTIC_OCEAN, LISBON, SEA);
    addLink(hv, ATLANTIC_OCEAN, GALWAY, SEA);

    // BAY_OF_BISCAY
    addLink(hv, BAY_OF_BISCAY, ATLANTIC_OCEAN, SEA);
    addLink(hv, BAY_OF_BISCAY, BORDEAUX, SEA);
    addLink(hv, BAY_OF_BISCAY, SANTANDER, SEA);
    addLink(hv, BAY_OF_BISCAY, NANTES, SEA);

    // MEDITERRANEAN_SEA
    addLink(hv, MEDITERRANEAN_SEA, ATLANTIC_OCEAN, SEA);
    addLink(hv, MEDITERRANEAN_SEA, TYRRHENIAN_SEA, SEA);
    addLink(hv, MEDITERRANEAN_SEA, CAGLIARI, SEA);
    addLink(hv, MEDITERRANEAN_SEA, MARSEILLES, SEA);
    addLink(hv, MEDITERRANEAN_SEA, BARCELONA, SEA);
    addLink(hv, MEDITERRANEAN_SEA, ALICANTE, SEA);

    // TYRRHENIAN SEA
    addLink(hv, TYRRHENIAN_SEA, MEDITERRANEAN_SEA, SEA);
    addLink(hv, TYRRHENIAN_SEA, CAGLIARI, SEA);
    addLink(hv, TYRRHENIAN_SEA, GENOA, SEA);
    addLink(hv, TYRRHENIAN_SEA, ROME, SEA);
    addLink(hv, TYRRHENIAN_SEA, NAPLES, SEA);
    addLink(hv, TYRRHENIAN_SEA, IONIAN_SEA, SEA);

    // IONIAN SEA
    addLink(hv, IONIAN_SEA, BLACK_SEA, SEA);
    addLink(hv, IONIAN_SEA, VALONA, SEA);
    addLink(hv, IONIAN_SEA, ATHENS, SEA);
    addLink(hv, IONIAN_SEA, SALONICA, SEA);
    addLink(hv, IONIAN_SEA, ADRIATIC_SEA, SEA);
    addLink(hv, IONIAN_SEA, TYRRHENIAN_SEA, SEA);

    // ADRIATIC SEA
    addLink(hv, ADRIATIC_SEA, VENICE, SEA);
    addLink(hv, ADRIATIC_SEA, BARI, SEA);
    addLink(hv, ADRIATIC_SEA, IONIAN_SEA, SEA);

    // BLACK SEA
    addLink(hv, BLACK_SEA, IONIAN_SEA, SEA);
    addLink(hv, BLACK_SEA, VARNA, SEA);
    addLink(hv, BLACK_SEA, CONSTANTA, SEA);

    /* CITIES->SEA Connections */

    addLink(hv, CONSTANTA, BLACK_SEA, SEA);
    addLink(hv, VARNA, BLACK_SEA, SEA);
    addLink(hv, SALONICA, IONIAN_SEA, SEA);
    addLink(hv, ATHENS, IONIAN_SEA, SEA);
    addLink(hv, VALONA, IONIAN_SEA, SEA);
    addLink(hv, BARI, ADRIATIC_SEA, SEA);
    addLink(hv, VENICE, ADRIATIC_SEA, SEA);
    addLink(hv, NAPLES, TYRRHENIAN_SEA, SEA);
    addLink(hv, ROME, TYRRHENIAN_SEA, SEA);
    addLink(hv, GENOA, TYRRHENIAN_SEA, SEA);
    addLink(hv, CAGLIARI, TYRRHENIAN_SEA, SEA);
    addLink(hv, CAGLIARI, MEDITERRANEAN_SEA, SEA);
    addLink(hv, MARSEILLES, MEDITERRANEAN_SEA, SEA);
    addLink(hv, BARCELONA, MEDITERRANEAN_SEA, SEA);
    addLink(hv, ALICANTE, MEDITERRANEAN_SEA, SEA);
    addLink(hv, CADIZ, ATLANTIC_OCEAN, SEA);
    addLink(hv, LISBON, ATLANTIC_OCEAN, SEA);
    addLink(hv, GALWAY, ATLANTIC_OCEAN, SEA);
    addLink(hv, SANTANDER, BAY_OF_BISCAY, SEA);
    addLink(hv, BORDEAUX, BAY_OF_BISCAY, SEA);
    addLink(hv, NANTES, BAY_OF_BISCAY, SEA);
    addLink(hv, LONDON, ENGLISH_CHANNEL, SEA);
    addLink(hv, PLYMOUTH, ENGLISH_CHANNEL, SEA);
    addLink(hv, LE_HAVRE, ENGLISH_CHANNEL, SEA);
    addLink(hv, SWANSEA, IRISH_SEA, SEA);
    addLink(hv, LIVERPOOL, IRISH_SEA, SEA);
    addLink(hv, DUBLIN, IRISH_SEA, SEA);
    addLink(hv, EDINBURGH, NORTH_SEA, SEA);
    addLink(hv, AMSTERDAM, NORTH_SEA, SEA);
    addLink(hv, HAMBURG, NORTH_SEA, SEA);
}
