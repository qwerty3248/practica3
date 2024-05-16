# include "AIPlayer.h"
# include "Parchis.h"

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 3;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta

bool AIPlayer::move(){
    cout << "Realizo un movimiento automatico" << endl;

    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

//aparentemente funciona 😡😡
double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristic)(const Parchis &, int)) const
{
    //Si la profundidad es igual al la profundidad_max o se acabo el juego damos la puntuacion
    if (profundidad == profundidad_max || actual.gameOver()){
        return heuristic(actual,jugador);
    }

    //🚗🚗
    bool Max_verstappen = actual.getCurrentPlayerId() == jugador; //Nodo max ?
    double valor; //valor que devuelve el nene

    ParchisBros hijos = actual.getChildren();//nenes

    for (auto it = hijos.begin(); it != hijos.end(); ++it)
    {   
        //Miramos que nos devuelve el nen
        valor = Poda_AlfaBeta(*it,jugador,profundidad+1,profundidad_max,c_piece,id_piece,dice,alpha,beta,heuristic);
        if (Max_verstappen)//Si somos max 
        {   
            if (alpha < valor) // Si el nuevo valor es mejor que alpha lo actualizo
            {
                alpha = valor;
                if (profundidad == 0) // SI es el primero comgemos la jugada
                {   
                    c_piece = it.getMovedColor();
                    id_piece = it.getMovedPieceId();
                    dice = it.getMovedDiceValue();                  
                }
            }

            if (alpha >= beta)//Si podamos devolvemos beta 
            {
                //cout << "Podo alpha\n";
                return beta;

            }

        }
        else //Si no es maximo es un minimo
        {
            if (beta > valor)//Si el valor es menor se actualiza
            {
                beta = valor;
            }

            if (beta <= alpha)//Si se poda
            {
                //cout<<"Podo beta\n";
                return alpha;
            }
        }

    }//Salida del for

    if (Max_verstappen)
    {
        return alpha;
    }
    else
    {
        return beta;
    } 

}

void AIPlayer::think(color & c_piece, int & id_piece, int & dice) const{
    // IMPLEMENTACIÓN INICIAL DEL AGENTE
    // Esta implementación realiza un movimiento aleatorio.
    // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente
    //como lo que se muestran al final de la función.

    // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores,
    //respectivamente, de:
    // - color de ficha a mover
    // - identificador de la ficha que se va a mover
    // - valor del dado con el que se va a mover la ficha.

    // El id de mi jugador actual.
    /*int player = actual->getCurrentPlayerId();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<tuple<color, int>> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableNormalDices(player);
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(player, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if (current_pieces.size() > 0)
    {
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]); // get<i>(tuple<...>) me devuelve el i-ésimo
        c_piece = get<0>(current_pieces[random_id]);  // elemento de la tupla
    }
    else
    {
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }*/


    
    // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.

    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
    // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
    //valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
    //cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    // ----------------------------------------------------------------- //

    // Si quiero poder manejar varias heurísticas, puedo usar la variable id del agente para usar una u otra.
    switch(id){
        case 0:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
            break;
        case 1:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion1);
            break;
        case 2:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion2);
            break;
    }
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    
}



double AIPlayer::ValoracionTest(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.


    int ganador = estado.getWinner();
    int oponente = (jugador+1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {   
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_jugador += 5;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

double AIPlayer::MiValoracion1(const Parchis &estado, int jugador){
    int ganador = estado.getWinner();
    int oponente = (jugador+1)%2;

    pair <color,int> piezasComidas;
    //Si gano yo devuelvo más infinito o menos infinito si pierdo
    if (ganador == jugador)
    {
        return ganador;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        double puntuacion_jugador = 0.0;

        if (estado.getCurrentPlayerId() == jugador)
        {
            if (estado.isGoalMove())//SI mete ficha
            {   
                puntuacion_jugador += 30 ; // Los puntuajes no son definitivos todavia
            }

            if (estado.isEatingMove())//Si se come una ficha
            {
                pair<color,int> piezasComidas = estado.eatenPiece();

                //Ahora miramos que no se una de las suyas porque puede comerse una de sus dos colores
                if (piezasComidas.first == my_colors[0] || piezasComidas.first == my_colors[1]) //Si es uno de mis dos colores afecta negativamente
                {
                    puntuacion_jugador -= 5; //Le devo restar o solo sumo, esto tengo que preguntarlo
                }
                else
                {
                    puntuacion_jugador += 20; //Suma muy positivamente comerse la de otros
                }

            }
            //Si revota en la meta vale menos porque prefiero que la meta con los movimientos justos
            if (estado.goalBounce())
            {
                puntuacion_jugador += 1;
            }

        }

        //Aqui deberia ir la parte de los dados para el jugador





        //Hasta aqui la pàrte de los dados del jugador, pensarlo algo mejor

        //Recorrer los colores como en el test del tutorial
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            puntuacion_jugador += estado.piecesAtHome(c) * 2;//Afecta MUY negativamente
            //Recorremos las fichas de ese color
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.getBoard().getPiece(c,j).get_box().type == goal)
                {
                    puntuacion_jugador += 100;//MUY positivamente
                }
                else
                {
                    if (estado.isSafePiece(c,j))
                    {
                        puntuacion_jugador += 10;
                    }
                    if (estado.piecesAtGoal(c)== 0)
                    {
                        puntuacion_jugador += (100-estado.distanceToGoal(c,i)) * 0.2;//Si tengo una es bueno pero depende de la distancia, el 0.2 porque si
                    }
                    else if (estado.piecesAtGoal(c) == 1)
                    {
                        puntuacion_jugador += (100-estado.distanceToGoal(c,i)) * 0.6;//Aqui aumenta el 0.2 porque estamos más cerca de la victoria
                    }
                    else if (estado.piecesAtGoal(c) == 2)
                    {
                        puntuacion_jugador += (100-estado.distanceToGoal(c,i)) * 4;//Meterla ya porque nos dara la victoria
                    }
                }
            }
        }
        //Fin de la parte del jugador
        
        //Aqui va lo del oponente que es hacer lo mismo que el jugador
        double puntuacion_oponente = 0.0;

        if (estado.getCurrentPlayerId() == oponente)
        {
            if (estado.isGoalMove())
            {
                puntuacion_oponente += 30;
            }

            if (estado.isEatingMove())
            {
                piezasComidas = estado.eatenPiece();

                if (piezasComidas.first == op_colors[0] || piezasComidas.first == op_colors[1])
                {
                    puntuacion_oponente += -5;
                }
                else
                {
                    puntuacion_oponente += 20;
                }
            }

            if (estado.goalBounce())
            {
                puntuacion_oponente += 1;
            }
            
        }
        //Parte de dados del oponente




        //Fin de parte de dados del oponente

        for (int i = 0; i < op_colors.size(); i++)
            {
                color c = op_colors[i];
                puntuacion_oponente += estado.piecesAtHome(c) * 2;//Afecta MUY negativamente
                for (int j = 0; j < num_pieces; j++)
                {
                    if (estado.getBoard().getPiece(c,j).get_box().type == goal)
                    {
                        puntuacion_oponente += 100;
                    }
                    else
                    {
                        if (estado.isSafePiece(c,j))
                        {
                            puntuacion_oponente += 10;
                        }
                        if (estado.piecesAtGoal(c)== 0)
                        {
                            puntuacion_oponente += (100-estado.distanceToGoal(c,i)) * 0.2;//Si tengo una es bueno pero depende de la distancia, el 0.2 porque si
                        }
                        else if (estado.piecesAtGoal(c) == 1)
                        {
                            puntuacion_oponente += (100-estado.distanceToGoal(c,i)) * 0.6;//Aqui aumenta el 0.2 porque estamos más cerca de la victoria
                        }
                        else if (estado.piecesAtGoal(c) == 2)
                        {
                            puntuacion_oponente += (100-estado.distanceToGoal(c,i)) * 4;//Meterla ya porque nos dara la victoria
                        } 
                    }
                }
            }
        //Aqui termina el oponente su codigo
        //Hacemos lo mismo que en la valoracion que nos dan hecho 
        return puntuacion_jugador - puntuacion_oponente;

    }
}
double AIPlayer::MiValoracion2(const Parchis &estado, int jugador){
    return 0.0;
}





