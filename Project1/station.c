#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "station.h"

/* Creeaza o gara cu un numar fix de peroane.
 * 
 * platforms_no: numarul de peroane ale garii
 *
 * return: gara creata
 */
TrainStation *open_train_station(int platforms_no)
{
    TrainStation *station = (TrainStation *)malloc(sizeof(TrainStation));
    station->platforms_no = platforms_no;
    station->platforms = (Train **)calloc(station->platforms_no, sizeof(Train *));
    return station;
}

/* Elibereaza memoria alocata pentru gara.
 *
 * station: gara existenta
 */
void close_train_station(TrainStation *station)
{
    if (station)
    {
        for (int i = 0; i < station->platforms_no; i++)
        {
            TrainCar *aux;

            if (station->platforms[i])
            {
                while (station->platforms[i]->train_cars != NULL)
                {
                    aux = station->platforms[i]->train_cars;
                    station->platforms[i]->train_cars = station->platforms[i]->train_cars->next;
                    free(aux);
                }
            }

            free(station->platforms[i]);
        }
        free(station->platforms);
        free(station);
    }
}

/* Afiseaza trenurile stationate in gara.
 *
 * station: gara existenta
 * f: fisierul in care se face afisarea
 */
void show_existing_trains(TrainStation *station, FILE *f)
{
    if (station)
    {
        for (int i = 0; i < station->platforms_no; i++)
        {
            fprintf(f, "%d: ", i);

            if (station->platforms[i])
            {
                TrainCar *aux = station->platforms[i]->train_cars;
                fprintf(f, "(%d)", station->platforms[i]->locomotive_power);
                while (aux != NULL)
                {
                    fprintf(f, "-|%d|", aux->weight);
                    aux = aux->next;
                }
                free(aux);
            }

            fprintf(f, "\n");
        }
    }
}

/* Adauga o locomotiva pe un peron.
 * 
 * station: gara existenta
 * platform: peronul pe care se adauga locomotiva
 * locomotive_power: puterea de tractiune a locomotivei
 */
void arrive_train(TrainStation *station, int platform, int locomotive_power)
{
    if (station)
    {
        if (platform < station->platforms_no && platform >= 0)
        {
            if (station->platforms[platform] == NULL)
            {
                station->platforms[platform] = (Train *)malloc(sizeof(Train));
                station->platforms[platform]->locomotive_power = locomotive_power;
                station->platforms[platform]->train_cars = NULL;
            }
        }
    }
}

/* Elibereaza un peron.
 * 
 * station: gara existenta
 * platform: peronul de pe care pleaca trenul
 */
void leave_train(TrainStation *station, int platform)
{
    if (station)
    {
        if (platform >= 0 && platform < station->platforms_no)
        {
            if (station->platforms[platform])
            {
                TrainCar *aux;
                while (station->platforms[platform]->train_cars != NULL)
                {
                    aux = station->platforms[platform]->train_cars;
                    station->platforms[platform]->train_cars = station->platforms[platform]->train_cars->next;
                    free(aux);
                }
                free(station->platforms[platform]);
                station->platforms[platform] = NULL;
            }
        }
    }
}

/* Adauga un vagon la capatul unui tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului adaugat
 */
void add_train_car(TrainStation *station, int platform, int weight)
{
    if (station)
    {
        if (platform >= 0 && platform < station->platforms_no)
        {
            if (station->platforms[platform])
            {
                if (station->platforms[platform]->train_cars)
                {
                    //  verificam daca este primul vagon
                    TrainCar *new = malloc(sizeof(TrainCar));
                    new->next = NULL;
                    new->weight = weight;
                    TrainCar *aux = station->platforms[platform]->train_cars;
                    while (aux->next != NULL)
                    {
                        aux = aux->next;
                    }
                    aux->next = new;
                }
                else
                {
                    station->platforms[platform]->train_cars = malloc(sizeof(TrainCar));
                    station->platforms[platform]->train_cars->next = NULL;
                    station->platforms[platform]->train_cars->weight = weight;
                }
            }
        }
    }
}

/* Scoate vagoanele de o anumita greutate dintr-un tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului scos
 */
void remove_train_cars(TrainStation *station, int platform, int weight)
{
    if (station)
    {
        if (platform >= 0 && platform < station->platforms_no)
        {
            if (station->platforms[platform])
            {
                TrainCar *aux = station->platforms[platform]->train_cars;
                TrainCar *supl;
                //  eliminam vagoanele de la inceput care au greutatea corespunzatoare
                while (aux != NULL && aux->weight == weight)
                {
                    station->platforms[platform]->train_cars = station->platforms[platform]->train_cars->next;
                    free(aux);
                    aux = station->platforms[platform]->train_cars;
                }
                // verificam restul vagoanelor si le eliminam
                if (aux != NULL)
                {
                    while (aux->next != NULL)
                    {
                        if (aux->next->weight == weight)
                        {
                            supl = aux->next;
                            aux->next = supl->next;
                            free(supl);
                        }
                        else
                        {
                            aux = aux->next;
                        }
                    }
                }
            }
        }
    }
}

/* Muta o secventa de vagoane dintr-un tren in altul.
 * 
 * station: gara existenta
 * platform_a: peronul pe care se afla trenul de unde se scot vagoanele
 * pos_a: pozitia primului vagon din secventa
 * cars_no: lungimea secventei
 * platform_b: peronul pe care se afla trenul unde se adauga vagoanele
 * pos_b: pozitia unde se adauga secventa de vagoane
 */
void move_train_cars(TrainStation *station, int platform_a, int pos_a,
                     int cars_no, int platform_b, int pos_b)
{
    if (station && cars_no > 0 && pos_a > 0 && pos_b > 0)
    {
        if (platform_a >= 0 && platform_a < station->platforms_no &&
            platform_b >= 0 && platform_b < station->platforms_no)
        {
            if (station->platforms[platform_a]->train_cars)
            {
                // calculam lungimile peroanelor
                int len1 = 1, len2 = 1;
                TrainCar *len = station->platforms[platform_a]->train_cars;
                TrainCar *lenn = station->platforms[platform_b]->train_cars;
                while (len != NULL)
                {
                    len = len->next;
                    len1++;
                }
                while (lenn != NULL)
                {
                    lenn = lenn->next;
                    len2++;
                }
                if (pos_a + cars_no <= len1 && pos_b <= len2)
                {
                    TrainCar *aux1 = station->platforms[platform_a]->train_cars;
                    TrainCar *aux2 = station->platforms[platform_a]->train_cars;
                    TrainCar *Beg, *Fin;
                    if (pos_a == 1)
                    {
                        //  verificam daca se muta primul vagon
                        for (int i = 1; i < cars_no; i++)
                        {
                            aux2 = aux2->next;
                        }
                        Beg = aux1;
                        Fin = aux2;
                        station->platforms[platform_a]->train_cars = aux2->next;
                    }
                    else
                    {
                        //  memoram de unde incepe secventa care trebuie mutata
                        for (int i = 2; i < pos_a; i++)
                        {
                            aux1 = aux1->next;
                        }
                        aux2 = aux1;
                        for (int i = 0; i < cars_no; i++)
                        {
                            aux2 = aux2->next;
                        }
                        Beg = aux1->next;
                        Fin = aux2;
                        aux1->next = aux2->next;
                    }

                    TrainCar *aux1fin = station->platforms[platform_b]->train_cars;
                    TrainCar *aux2fin = station->platforms[platform_b]->train_cars;
                    if (pos_b == 1)
                    {
                        // o mutam pe prima pozitie
                        Fin->next = station->platforms[platform_b]->train_cars;
                        station->platforms[platform_b]->train_cars = Beg;
                    }
                    else
                    {
                        for (int i = 2; i < pos_b; i++)
                        {
                            aux1fin = aux1fin->next;
                        }

                        aux2fin = aux1fin->next;
                        aux1fin->next = Beg;
                        Fin->next = aux2fin;
                    }
                }
            }
        }
    }
}

/* Gaseste trenul cel mai rapid.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_express_train(TrainStation *station)
{
    if (station)
    {
        int *weight = calloc(station->platforms_no, sizeof(int));
        int *final = calloc(station->platforms_no, sizeof(int));

        for (int i = 0; i < station->platforms_no; i++)
        {
            if (station->platforms[i])
            {
                TrainCar *aux = station->platforms[i]->train_cars;
                while (aux != NULL)
                {
                    weight[i] = weight[i] + aux->weight;
                    aux = aux->next;
                }
                //  calculam diferenta dintre puterea locomotivei si greutate
                final[i] = station->platforms[i]->locomotive_power - weight[i];
            }
        }
        int k = 0, max = 0;
        for (int i = 0; i < station->platforms_no; i++)
        {
            if (max < final[i])
            {
                //  cautam valoarea maxima, adica trenul cel mai liber
                max = final[i];
                k = i;
            }
        }

        free(weight);
        free(final);
        return k;
    }
}

/* Gaseste trenul supraincarcat.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_overload_train(TrainStation *station)
{
    if (station)
    {
        int *weight = calloc(station->platforms_no, sizeof(int));
        int *final = calloc(station->platforms_no, sizeof(int));

        for (int i = 0; i < station->platforms_no; i++)
        {
            if (station->platforms[i])
            {
                TrainCar *aux = station->platforms[i]->train_cars;
                while (aux != NULL)
                {
                    weight[i] = weight[i] + aux->weight;
                    aux = aux->next;
                }
                final[i] = station->platforms[i]->locomotive_power - weight[i];
            }
        }
        int k = -1;
        for (int i = 0; i < station->platforms_no; i++)
        {
            //  verificam daca greutatea vagoanelor este mai mare decat puterea locomotivei
            if (final[i] < 0)
            {
                k = i;
            }
        }

        free(weight);
        free(final);
        return k;
    }
}

/* Gaseste trenul cu incarcatura optima.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_optimal_train(TrainStation *station)
{
    if (station)
    {
        int *weight = calloc(station->platforms_no, sizeof(int));
        int *final = calloc(station->platforms_no, sizeof(int));

        for (int i = 0; i < station->platforms_no; i++)
        {
            if (station->platforms[i])
            {
                TrainCar *aux = station->platforms[i]->train_cars;
                while (aux != NULL)
                {
                    weight[i] = weight[i] + aux->weight;
                    aux = aux->next;
                }
                final[i] = station->platforms[i]->locomotive_power - weight[i];
            }
        }
        int k = -1, min = 100;
        for (int i = 0; i < station->platforms_no; i++)
        {
            if (final[i] > 0)
            {
                if (min > final[i])
                {
                    //  cautam valoarea minima a diferentei dintre puterea locomotivei
                    //  si greutatea vagoanelor
                    min = final[i];
                    k = i;
                }
            }
        }

        free(weight);
        free(final);
        return k;
    }
}

/* Gaseste trenul cu incarcatura nedistribuita bine.
 * 
 * station: gara existenta
 * cars_no: lungimea secventei de vagoane
 *
 * return: peronul pe care se afla trenul
 */
int find_heaviest_sequence_train(TrainStation *station, int cars_no, TrainCar **start_car)
{
    if (station)
    {
        int weight[100][100];
        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 100; j++)
            {
                weight[i][j] = 0;
            }
        }

        for (int i = 0; i < station->platforms_no; i++)
        {
            if (station->platforms[i])
            {
                int len = 0;
                TrainCar *aux = station->platforms[i]->train_cars;
                while (aux != NULL)
                {
                    len++;
                    aux = aux->next;
                }
                aux = station->platforms[i]->train_cars;
                int k = 0;
                while (k + cars_no <= len)
                {
                    for (int j = 0; j < cars_no; j++)
                    {
                        //  putem in matrice[i][k] valoarea greutatii de pe peronul i, incepand
                        //  cu vagonul k a cars_no vagoane
                        weight[i][k] = weight[i][k] + aux->weight;
                        aux = aux->next;
                    }
                    k++;
                    aux = station->platforms[i]->train_cars;
                    for (int m = 0; m < k; m++)
                    {
                        aux = aux->next;
                    }
                }
            }
        }
        int max = 0;
        int peron = -1, start = 0;
        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 100; j++)
            {
                if (max < weight[i][j])
                {
                    //  cautam cea mai grea secventa si memoram numarul peronului
                    //  si inceputul secventei
                    max = weight[i][j];
                    peron = i;
                    start = j;
                }
            }
        }
        if (peron >= 0)
        {
            TrainCar *point = station->platforms[peron]->train_cars;
            for (int i = 0; i < start; i++)
            {
                point = point->next;
            }
            *start_car = point;
        }
        else
        {
            *start_car = NULL;
        }
        return peron;
    }
}

/* Ordoneaza vagoanele dintr-un tren in ordinea descrescatoare a greutatilor.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 */
void order_train(TrainStation *station, int platform)
{
    if (station && platform >= 0 && platform < station->platforms_no)
    {
        if (station->platforms[platform])
        {
            int len = 0;
            TrainCar *aux = station->platforms[platform]->train_cars, *final;
            while (aux != NULL)
            {
                if (aux->next == NULL)
                {
                    final = aux;
                }
                aux = aux->next;
                len++;
            }
            if (len > 1)
            {
                aux = station->platforms[platform]->train_cars;
                if (len == 2)
                {
                    //  daca lungimea e 2 inversam vagoanele
                    if (aux->weight < aux->next->weight)
                    {
                        TrainCar *last = aux->next;
                        last->next = aux;
                        aux->next = NULL;
                        station->platforms[platform]->train_cars = last;
                    }
                }
                else
                {
                    aux = station->platforms[platform]->train_cars;
                    int max = 0;
                    TrainCar *maxpoint;
                    while (aux->next != NULL)
                    {
                        if (max < aux->next->weight)
                        {
                            max = aux->next->weight;
                            maxpoint = aux; //  gasim cel mai greu vagon
                        }
                        aux = aux->next;
                    }
                    aux = station->platforms[platform]->train_cars;
                    if (aux->weight < max)
                    {
                        //  mutam cel mai greu vagon la inceputul listei
                        //  schimbam si head-ul listei
                        final->next = aux;
                        station->platforms[platform]->train_cars = maxpoint->next;
                        maxpoint->next = NULL;
                    }
                    aux = station->platforms[platform]->train_cars;
                    int k = 0;
                    //  facem sortarea celorlalte vagoane
                    while (k < len * len * 2)
                    {
                        TrainCar *prev = aux, *inter = prev->next, *last = inter->next;
                        if (inter->weight < last->weight)
                        {
                            prev->next = last;
                            inter->next = last->next;
                            last->next = inter;
                        }
                        else
                        {
                            aux = aux->next;
                        }
                        if (aux->next->next == NULL)
                        {
                            aux = station->platforms[platform]->train_cars;
                        }
                        k++;
                    }
                }
            }
        }
    }
}

/* Scoate un vagon din trenul supraincarcat.
 * 
 * station: gara existenta
 */
void fix_overload_train(TrainStation *station)
{
    if (station)
    {
        int *weight = calloc(station->platforms_no, sizeof(int));
        int *final = calloc(station->platforms_no, sizeof(int));
        int optim[100];
        for (int i = 0; i < 100; i++)
        {
            optim[i] = -1;
        }

        for (int i = 0; i < station->platforms_no; i++)
        {
            if (station->platforms[i])
            {
                TrainCar *aux = station->platforms[i]->train_cars;
                while (aux != NULL)
                {
                    weight[i] = weight[i] + aux->weight;
                    aux = aux->next;
                }
                final[i] = station->platforms[i]->locomotive_power - weight[i];
            }
        }
        int k = -1;
        for (int i = 0; i < station->platforms_no; i++)
        {
            if (final[i] < 0)
            {
                k = i;
                //  gasim peronul pe care se afla trenul supraincarcat
            }
        }
        if (k > 0)
        {
            TrainCar *aux = station->platforms[k]->train_cars;
            int i = 0, min = 100, fixtrain = 0;

            while (aux != NULL)
            {
                //  calculam care ar fi diferenta dintre puterea locomotivei si greutatea vagoanelor
                //  pentru fiecare caz in care scoatem un vagon
                optim[i] = final[k] + aux->weight;
                aux = aux->next;
                i++;
            }

            for (int i = 0; i < 100; i++)
            {
                if (optim[i] >= 0)
                {
                    if (min > optim[i])
                    {
                        //  gasim cea mai buna varianta (vagonul care trebuie scos)
                        min = optim[i];
                        fixtrain = i;
                    }
                }
            }
            aux = station->platforms[k]->train_cars;
            TrainCar *prev;
            if (fixtrain == 0)
            {
                //  cazul in care e primul vagon si mutam si head-ul listei
                station->platforms[k]->train_cars = aux->next;
                free(aux);
            }
            else
            {
                for (int i = 0; i < fixtrain; i++)
                {
                    //  folosim un pointer la vagon pentru a-l elimina
                    prev = aux;
                    aux = aux->next;
                }
                prev->next = aux->next;
                free(aux);
            }
        }

        free(weight);
        free(final);
    }
}
