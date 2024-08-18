#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 3

typedef enum {
    EMPTY, X, O
} Player;
typedef enum {
    MODE_HUMAN, MODE_COMPUTER
} GameMode;

typedef struct {
    Player grid[GRID_SIZE][GRID_SIZE];
    Player current_player;
    GameMode mode;
    GtkWidget *buttons[GRID_SIZE][GRID_SIZE];
    gboolean game_over;
} GameState;

static void reset_game(GameState *state) {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            state->grid[row][col] = EMPTY;
            gtk_button_set_label(GTK_BUTTON(state->buttons[row][col]), "");
            gtk_widget_set_sensitive(state->buttons[row][col], TRUE);
        }
    }
    state->current_player = X;
    state->game_over = FALSE;
}

static gboolean check_winner(GameState *state, Player player) {
    // 检查行、列和对角线
    for (int i = 0; i < GRID_SIZE; i++) {
        if (state->grid[i][0] == player && state->grid[i][1] == player && state->grid[i][2] == player) {
            return TRUE;
        }
        if (state->grid[0][i] == player && state->grid[1][i] == player && state->grid[2][i] == player) {
            return TRUE;
        }
    }
    if (state->grid[0][0] == player && state->grid[1][1] == player && state->grid[2][2] == player) {
        return TRUE;
    }
    if (state->grid[0][2] == player && state->grid[1][1] == player && state->grid[2][0] == player) {
        return TRUE;
    }
    return FALSE;
}

static gboolean check_draw(GameState *state) {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (state->grid[row][col] == EMPTY) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

static void end_game(GameState *state, const char *message) {
    state->game_over = TRUE;
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            gtk_widget_set_sensitive(state->buttons[row][col], FALSE);
        }
    }

    GtkWidget *dialog = gtk_message_dialog_new_with_markup(
            GTK_WINDOW(gtk_widget_get_ancestor(state->buttons[0][0], GTK_TYPE_WINDOW)),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "%s", message
    );

    gtk_window_set_transient_for(GTK_WINDOW(dialog),
                                 GTK_WINDOW(gtk_widget_get_ancestor(state->buttons[0][0], GTK_TYPE_WINDOW)));
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_destroy), NULL);
    gtk_widget_show(dialog);
//    gtk_window_set_child(GTK_WINDOW(dialog), dialog);
//    gtk_window_present(GTK_WINDOW(dialog));
}

static void computer_move(GameState *state) {
    int row, col;
    do {
        row = rand() % GRID_SIZE;
        col = rand() % GRID_SIZE;
    } while (state->grid[row][col] != EMPTY);

    state->grid[row][col] = O;
    gtk_button_set_label(GTK_BUTTON(state->buttons[row][col]), "O");

    if (check_winner(state, O)) {
        end_game(state, "Computer wins!");
    } else if (check_draw(state)) {
        end_game(state, "It's a draw!");
    } else {
        state->current_player = X;
    }
}

static void on_button_click(GtkWidget *widget, gpointer data) {
    GameState *state = (GameState *) data;
    if (state->game_over) return;

    int row, col;
    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            if (state->buttons[row][col] == widget) {
                break;
            }
        }
        if (col < GRID_SIZE) break;
    }

    if (state->grid[row][col] != EMPTY) {
        return;
    }

    state->grid[row][col] = state->current_player;
    gtk_button_set_label(GTK_BUTTON(widget), state->current_player == X ? "X" : "O");

    if (check_winner(state, state->current_player)) {
        end_game(state, state->current_player == X ? "Player X wins!" : "Player O wins!");
    } else if (check_draw(state)) {
        end_game(state, "It's a draw!");
    } else if (state->mode == MODE_COMPUTER) {
        state->current_player = O;
        computer_move(state);
    } else {
        state->current_player = (state->current_player == X) ? O : X;
    }
}

static void on_reset_button_click(GtkWidget *widget, gpointer data) {
    GameState *state = (GameState *) data;
    reset_game(state);
}

static void on_mode_button_click(GtkWidget *widget, gpointer data) {
    GameState *state = (GameState *) data;
    if (state->mode == MODE_HUMAN) {
        state->mode = MODE_COMPUTER;
        gtk_button_set_label(GTK_BUTTON(widget), "Mode: Computer");
    } else {
        state->mode = MODE_HUMAN;
        gtk_button_set_label(GTK_BUTTON(widget), "Mode: Human");
    }
    reset_game(state);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GameState *state = g_new0(GameState, 1);
    state->current_player = X;
    state->mode = MODE_HUMAN;
    state->game_over = FALSE;
    srand(time(NULL));

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tic Tac Toe");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 400);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_box_append(GTK_BOX(vbox), grid);

    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            GtkWidget *button = gtk_button_new();
            state->buttons[row][col] = button;
            state->grid[row][col] = EMPTY;

            gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);
            g_signal_connect(button, "clicked", G_CALLBACK(on_button_click), state);
        }
    }

    GtkWidget *reset_button = gtk_button_new_with_label("Reset");
    gtk_box_append(GTK_BOX(vbox), reset_button);
    g_signal_connect(reset_button, "clicked", G_CALLBACK(on_reset_button_click), state);

    GtkWidget *mode_button = gtk_button_new_with_label("Mode: Human");
    gtk_box_append(GTK_BOX(vbox), mode_button);
    g_signal_connect(mode_button, "clicked", G_CALLBACK(on_mode_button_click), state);

    gtk_widget_set_vexpand(grid, TRUE);
    gtk_widget_set_hexpand(grid, TRUE);
//    gtk_widget_show(window);
    gtk_window_set_child(GTK_WINDOW(window), vbox);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.eu.neolux.tictactoe", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
