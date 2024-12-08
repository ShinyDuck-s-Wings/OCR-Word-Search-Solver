#include <gtk/gtk.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "rotaimg.h"
#ifndef G_UNUSED
#define G_UNUSED(p) ((void)(p))
#endif

int angle = 0;
guint timer_id = 0;
GtkBuilder *builder;
const int WIDTH = 300;
const int HEIGHT = 300;
char *filename = NULL;
GtkWidget *label;
int max_width = 600;
int max_height = 500;


GdkPixbuf *resize_pixbuf(GdkPixbuf *original_pixbuf) {
    if (!original_pixbuf) {
        g_printerr("Le GdkPixbuf fourni est NULL.\n");
        return NULL;
    }

    int original_width = gdk_pixbuf_get_width(original_pixbuf);
    int original_height = gdk_pixbuf_get_height(original_pixbuf);

    // Calculer les dimensions redimensionnées
    double scale_width = (double)max_width / original_width;
    double scale_height = (double)max_height / original_height;
    double scale = scale_width < scale_height ? scale_width : scale_height;

    int new_width = original_width * scale;
    int new_height = original_height * scale;

    // Redimensionner l'image
    GdkPixbuf *resized_pixbuf = gdk_pixbuf_scale_simple(
        original_pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);

    if (!resized_pixbuf) {
        g_printerr("Erreur lors du redimensionnement de l'image\n");
    }

    return resized_pixbuf;
}

void on_load_button_clicked(GtkButton *button, gpointer user_data) {
    angle = 0;
    G_UNUSED(button);
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open file",
                                        GTK_WINDOW(user_data),
                                        action,
                                        "Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        "Open",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);

        filename = gtk_file_chooser_get_filename(chooser);
        if (filename == NULL) {
            g_print("Error: No file selected.\n");
            gtk_widget_destroy(dialog);
            return;
        }

        const char *extension = strrchr(filename, '.');
        if (strcmp(extension, ".jpg") == 0 ||
            strcmp(extension, ".jpeg") == 0 ||
            strcmp(extension, ".bmp") == 0 ||
            strcmp(extension, ".png") == 0) {

            GtkWidget *image_widget = GTK_WIDGET(gtk_builder_get_object(builder, "image_f"));

            // Load the image into pixbuf
            GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
            if (pixbuf == NULL) {
                g_print("Error: Failed to load image from file.\n");
                gtk_widget_destroy(dialog);
                return;
            }

            // Save the image
            char *exe_path = g_get_current_dir();
            char *output_path = g_build_filename(exe_path, "altimg.png", NULL);

            g_free(exe_path);
            GError *error = NULL;
            if (!gdk_pixbuf_save(pixbuf, output_path, "png", &error, NULL)) {
                g_print("Error saving altimg: %s\n", error->message);
                g_error_free(error);
            } else {
                g_print("Image copied and saved as: %s\n", output_path);
            }

            g_free(output_path);

            // Set the loaded pixbuf to the image widget
            pixbuf = resize_pixbuf(pixbuf);
            
            gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), pixbuf);

            // Reset the rotation angle when loading a new image
            g_object_set(image_widget, "angle", 10.0, NULL);

            // Unreference the pixbuf after use
            g_object_unref(pixbuf);
        }
        g_free(filename);
    }
    rotate_image("altimg.png", angle);
    gtk_widget_destroy(dialog);
}

//iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii
void on_Resolver_clicked(GtkButton *button, gpointer user_data) {
    G_UNUSED(button);
    G_UNUSED(user_data);

    pid_t pid = fork();

    if (pid == -1) {
        // Erreur lors du fork
        perror("fork");
        return;
    } else if (pid == 0) {
        // Processus enfant
        char *const args[] = { "../main/main", "rotimage.bmp", NULL };
        execvp(args[0], args);
        // Si execvp échoue
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        // Processus parent
        int status;
        waitpid(pid, &status, 0); // Attendre la fin du processus enfant

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            // Si le processus s'est terminé correctement
            GtkWidget *image_widget = GTK_WIDGET(gtk_builder_get_object(builder, "image_f"));
            GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("../imgf/imgres.bmp", NULL);
            
            if (pixbuf != NULL) {
            pixbuf = resize_pixbuf(pixbuf);
                gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), pixbuf);
            } else {
                g_warning("Erreur lors du chargement de l'image.");
            }
            printf("Image mise à jour avec succès.\n");
        } else {
            // Si le processus enfant a échoué
            g_warning("Erreur dans le processus externe.\n");
        }
    }
}

void on_filter_clicked(GtkButton *button, gpointer user_data) {

  G_UNUSED(button);
    G_UNUSED(user_data);

    
    char *const args[] = { "../neural_network/NeuralNetwork", "train", NULL };

    
    g_print("Exécution de : %s\n", args[0]);

    pid_t pid = fork();

    
     if (pid == 0) {
        
        int result = execvp(args[0], args);
        if (result == -1) {
            perror("Erreur lors de l'exécution du programme");
        }
        exit(0); 
    } else {
        
        g_print("Le processus parent continue d'exécuter l'application GTK.\n");
    }
}


// iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii


gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    switch (event->keyval) {
        case GDK_KEY_f:
            angle--; 
            break;
        case GDK_KEY_j:
            angle++;
            break;
        default:
            break;
    }
    
    rotate_image("altimg.png", angle);
    printf("was here true, %d %s\n", angle,filename);
    GtkWidget *image_widget = GTK_WIDGET(gtk_builder_get_object(builder, "image_f"));
            GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("rotimage.bmp", NULL);
            pixbuf = resize_pixbuf(pixbuf);
            gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), pixbuf);
            
    if(angle%360==0) angle = angle/360;
    char angle_text[32];
    snprintf(angle_text, sizeof(angle_text), "%d°", angle);

    
    gtk_label_set_text(GTK_LABEL(label), angle_text);

            
    return FALSE; 
     
}

// Gestion des événements de touche relâchée
gboolean on_key_release(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    switch (event->keyval) {
        case GDK_KEY_f:
        case GDK_KEY_j:
             // Arrêter le timer lorsque F ou J est relâché
            break;
        default:
            break;
            
    }
    printf("was here false \n");
    return FALSE; // Retourner FALSE pour propager l'événement
}





int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);
    

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "app.glade", NULL);

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    GtkWidget *button_load = GTK_WIDGET(gtk_builder_get_object(builder, "button_load"));
    g_signal_connect(button_load, "clicked", G_CALLBACK(on_load_button_clicked), window);


  GtkWidget *b_filterS = GTK_WIDGET(gtk_builder_get_object(builder, "b_filterS"));
  g_signal_connect(b_filterS, "clicked", G_CALLBACK(on_filter_clicked), NULL);
  
  GtkWidget *button_resolve = GTK_WIDGET(gtk_builder_get_object(builder, "button_resolve"));
  g_signal_connect(button_resolve, "clicked", G_CALLBACK(on_Resolver_clicked), NULL);
  
    label = GTK_WIDGET(gtk_builder_get_object(builder, "Langle"));   

    

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);
    g_signal_connect(window, "key-release-event", G_CALLBACK(on_key_release), NULL);

    gtk_widget_show(window);
    gtk_main();

    return 0;
}




