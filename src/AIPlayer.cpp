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
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion1);//Solo el 1
            break;
        case 2:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion2);//Solo el 1
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


// si lo hace: 👌 no lo hace: 𐢫
//EL nivel 1: primero  👌 | segundo 👌
//El nivel 2: primero  👌 | segundo 👌
//El nivel 3: primero   | segundo 
double AIPlayer::MiValoracion1(const Parchis &estado, int jugador) {
    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;
    const int CASILLASRECORRER = 68 + 7; // Las casillas que va a recorrer cada ficha

    if (ganador == jugador) {
        return gana; // Valor alto para la victoria
    } else if (ganador == oponente) {
        return pierde; // Valor bajo para la derrota
    } else {
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        double puntuacion_jugador = 0.0;

        // Encuentra el color del jugador con más piezas en la meta
        color max_goal_color;
        int max_pieces_at_goal = -1;
        for (int i = 0; i < my_colors.size(); i++) {
            color c = my_colors[i];
            int pieces_at_goal = estado.piecesAtGoal(c);
            if (pieces_at_goal > max_pieces_at_goal) {
                max_pieces_at_goal = pieces_at_goal;
                max_goal_color = c;
            }
        }

        for (int i = 0; i < my_colors.size(); i++) {
            color c = my_colors[i];

            // Penaliza piezas en casa, fomenta moverlas
            puntuacion_jugador -= estado.piecesAtHome(c) * 5;

            // Fomenta piezas en la meta con un peso mayor
            int pieces_at_goal = estado.piecesAtGoal(c);
            puntuacion_jugador += pieces_at_goal * 100;

            // Bonus adicional por tener dos piezas en la meta (ya que con tres se gana)
            if (pieces_at_goal == 2) {
                puntuacion_jugador += 200; // Incentivo por estar muy cerca de la victoria
            }   

            // Evalúa cada ficha individualmente
            for (int j = 0; j < num_pieces; j++) {
                int distance = estado.distanceToGoal(c, j);
                if (distance > 0) {
                    // Fomenta más el avance de las piezas del color con más piezas en la meta
                    double factor = (c == max_goal_color) ? 0.2 : 0.1;
                    puntuacion_jugador += (CASILLASRECORRER - distance) * factor;
                }
            }
        }

        // Factor de captura y movimiento a meta
        if (estado.getCurrentPlayerId() == jugador) {
            if (estado.isEatingMove()) {
                pair<color, int> Comidas = estado.eatenPiece();
                puntuacion_jugador += (Comidas.first == my_colors[0] || Comidas.first == my_colors[1]) ? 10 : 50;
            }
            if (estado.isGoalMove()) {
                puntuacion_jugador += 20;
            }
            if (estado.goalBounce()) {
                puntuacion_jugador -= 10; // Penaliza rebotar en la meta
            }
        }

        double puntuacion_oponente = 0.0;

        // Encuentra el color del oponente con más piezas en la meta
        color max_goal_color_op;
        int max_pieces_at_goal_op = -1;
        for (int i = 0; i < op_colors.size(); i++) {
            color c = op_colors[i];
            int pieces_at_goal = estado.piecesAtGoal(c);
            if (pieces_at_goal > max_pieces_at_goal_op) {
                max_pieces_at_goal_op = pieces_at_goal;
                max_goal_color_op = c;
            }
        }

        for (int i = 0; i < op_colors.size(); i++) {
            color c = op_colors[i];

            puntuacion_oponente -= estado.piecesAtHome(c) * 5;
            int pieces_at_goal = estado.piecesAtGoal(c);
            puntuacion_oponente += pieces_at_goal * 100;

            // Bonus adicional por tener dos piezas en la meta (ya que con tres se gana)
            if (pieces_at_goal == 2) {
                puntuacion_oponente += 200; // Incentivo por estar muy cerca de la victoria
            }

            for (int j = 0; j < num_pieces; j++) {
                int distance = estado.distanceToGoal(c, j);
                if (distance > 0) {
                    double factor = (c == max_goal_color_op) ? 0.2 : 0.1;
                    puntuacion_oponente += (CASILLASRECORRER - distance) * factor;
                }
            }
        }

        if (estado.getCurrentPlayerId() == oponente) {
            if (estado.isEatingMove()) {
                pair<color, int> Comidas = estado.eatenPiece();
                puntuacion_oponente += (Comidas.first == op_colors[0] || Comidas.first == op_colors[1]) ? 10 : 50;
            }
            if (estado.isGoalMove()) {
                puntuacion_oponente += 20;
            }
            if (estado.goalBounce()) {
                puntuacion_oponente -= 10;
            }
        }

        return puntuacion_jugador - puntuacion_oponente;
    }
}
//No gana al 3 ni pa tras 
double AIPlayer::MiValoracion2(const Parchis &estado, int jugador) {
    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;
    const int CASILLASRECORRER = 68 + 7; // Las casillas que va a recorrer cada ficha

    if (ganador == jugador) {
        return gana; // Valor alto para la victoria
    } else if (ganador == oponente) {
        return pierde; // Valor bajo para la derrota
    } else {
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        double puntuacion_jugador = 0.0;
        //Mirar el uso del dado especial
        /*if ()
        {
            puntuacion_jugador += estado.getPower(jugador) * 20; //Ajustar para todos los valores
        }*/

        // Encuentra el color del jugador con más piezas en la meta
        color max_goal_color;
        int max_pieces_at_goal = -1;
        for (int i = 0; i < my_colors.size(); i++) {
            color c = my_colors[i];
            int pieces_at_goal = estado.piecesAtGoal(c);
            if (pieces_at_goal > max_pieces_at_goal) {
                max_pieces_at_goal = pieces_at_goal;
                max_goal_color = c;
            }
        }

        for (int i = 0; i < my_colors.size(); i++) {
            color c = my_colors[i];

            // Penaliza piezas en casa, fomenta moverlas
            puntuacion_jugador -= estado.piecesAtHome(c) * 10;

            // Fomenta piezas en la meta con un peso mayor
            int pieces_at_goal = estado.piecesAtGoal(c);
            puntuacion_jugador += pieces_at_goal * 100;

            // Bonus adicional por tener dos piezas en la meta (ya que con tres se gana)
            if (pieces_at_goal == 2) {
                puntuacion_jugador += 200; // Incentivo por estar muy cerca de la victoria
            }

            // Evalúa cada ficha individualmente
            for (int j = 0; j < num_pieces; j++) {
                int distance = estado.distanceToGoal(c, j);
                if (distance > 0) {
                    // Fomenta más el avance de las piezas del color con más piezas en la meta
                    double factor = (c == max_goal_color) ? 0.2 : 0.1;
                    puntuacion_jugador += (CASILLASRECORRER - distance) * factor;
                }

                // Considerar si la ficha está en una casilla segura
                if (estado.isSafeBox(estado.getBoard().getPiece(c,j).get_box())) {
                    puntuacion_jugador += 10; // Bonificación por estar en una casilla segura
                }
            }
        }

        // Factor de captura y movimiento a meta
        if (estado.getCurrentPlayerId() == jugador) {
            if (estado.isEatingMove()) {
                pair<color, int> Comidas = estado.eatenPiece();
                puntuacion_jugador += (Comidas.first == my_colors[0] || Comidas.first == my_colors[1]) ? 20 : 50;
            }
            if (estado.isGoalMove()) {
                puntuacion_jugador += 50;
            }
            if (estado.goalBounce()) {
                puntuacion_jugador -= 20; // Penaliza rebotar en la meta
            }
        }

        double puntuacion_oponente = 0.0;

        // Encuentra el color del oponente con más piezas en la meta
        color max_goal_color_op;
        int max_pieces_at_goal_op = -1;
        for (int i = 0; i < op_colors.size(); i++) {
            color c = op_colors[i];
            int pieces_at_goal = estado.piecesAtGoal(c);
            if (pieces_at_goal > max_pieces_at_goal_op) {
                max_pieces_at_goal_op = pieces_at_goal;
                max_goal_color_op = c;
            }
        }

        for (int i = 0; i < op_colors.size(); i++) {
            color c = op_colors[i];

            puntuacion_oponente -= estado.piecesAtHome(c) * 10;
            int pieces_at_goal = estado.piecesAtGoal(c);
            puntuacion_oponente += pieces_at_goal * 100;

            // Bonus adicional por tener dos piezas en la meta (ya que con tres se gana)
            if (pieces_at_goal == 2) {
                puntuacion_oponente += 200; // Incentivo por estar muy cerca de la victoria
            }

            for (int j = 0; j < num_pieces; j++) {
                int distance = estado.distanceToGoal(c, j);
                if (distance > 0) {
                    double factor = (c == max_goal_color_op) ? 0.2 : 0.1;
                    puntuacion_oponente += (CASILLASRECORRER - distance) * factor;
                }

                // Considerar si la ficha está en una casilla segura
                if (estado.isSafeBox(estado.getBoard().getPiece(c,j).get_box())) {
                    puntuacion_oponente += 10; // Bonificación por estar en una casilla segura
                }
            }
        }

        if (estado.getCurrentPlayerId() == oponente) {
            if (estado.isEatingMove()) {
                pair<color, int> Comidas = estado.eatenPiece();
                puntuacion_oponente += (Comidas.first == op_colors[0] || Comidas.first == op_colors[1]) ? 20 : 50;
            }
            if (estado.isGoalMove()) {
                puntuacion_oponente += 50;
            }
            if (estado.goalBounce()) {
                puntuacion_oponente -= 20;
            }
        }

        return puntuacion_jugador - puntuacion_oponente;
    }
}








