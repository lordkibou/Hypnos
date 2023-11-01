package com.example.hypnosapp;

import static androidx.constraintlayout.helper.widget.MotionEffect.TAG;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.util.LruCache;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.result.ActivityResult;
import androidx.activity.result.ActivityResultCallback;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.android.volley.RequestQueue;
import com.android.volley.toolbox.ImageLoader;
import com.android.volley.toolbox.NetworkImageView;
import com.android.volley.toolbox.Volley;
import com.facebook.login.LoginManager;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.auth.UserProfileChangeRequest;

import java.util.Objects;

public class PerfilUsuarioActivity extends AppCompatActivity {

    FirebaseAuth firebaseAuth;
    FirebaseUser firebaseUser;
    String nombreUsuario, correoUsuario;
    TextView nombre, nombreApellidos, correo, contrasenya;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //obtenemos la sesión y el usuario:
        firebaseAuth = FirebaseAuth.getInstance();
        firebaseUser = firebaseAuth.getCurrentUser();

        //Obtención de datos del usuario:
        nombreUsuario = firebaseUser.getDisplayName();
        correoUsuario = firebaseUser.getEmail();
        Uri urlFoto = firebaseUser.getPhotoUrl();
        String proveedores = "";
        for (int n=0; n<firebaseUser.getProviderData().size(); n++){
            proveedores += firebaseUser.getProviderData().get(n).getProviderId()+", ";
        }

        if (proveedores.contains("google")) {
            //Si iniciamos sesión con Google:
            setContentView(R.layout.perfil_usuario_google_facebook);

            //PARA OBTENER LA FOTO DE PERFIL DEL USUARIO
            // Inicialización Volley
            RequestQueue colaPeticiones = Volley.newRequestQueue(this);
            ImageLoader lectorImagenes = new ImageLoader(colaPeticiones,
                    new ImageLoader.ImageCache() {
                        private final LruCache<String, Bitmap> cache =
                                new LruCache<String, Bitmap>(10);
                        public void putBitmap(String url, Bitmap bitmap) {
                            cache.put(url, bitmap);
                        }
                        public Bitmap getBitmap(String url) {
                            return cache.get(url);
                        }
                    });

            // Foto de usuario
            if (urlFoto != null) {
                NetworkImageView foto = (NetworkImageView) findViewById(R.id.fotoPerfil);
                foto.setImageUrl(urlFoto.toString(), lectorImagenes);
            }

            nombre = findViewById(R.id.inputNombre);
            nombre.setText(nombreUsuario);

            correo = findViewById(R.id.inputEmail);
            correo.setText(correoUsuario);

        }
        else if(proveedores.contains("facebook")){
            //Si iniciamos sesión con Facebook:
            setContentView(R.layout.perfil_usuario_google_facebook);

            //PARA OBTENER LA FOTO DE PERFIL DEL USUARIO
            // Inicialización Volley
            RequestQueue colaPeticiones = Volley.newRequestQueue(this);
            ImageLoader lectorImagenes = new ImageLoader(colaPeticiones,
                    new ImageLoader.ImageCache() {
                        private final LruCache<String, Bitmap> cache =
                                new LruCache<String, Bitmap>(10);
                        public void putBitmap(String url, Bitmap bitmap) {
                            cache.put(url, bitmap);
                        }
                        public Bitmap getBitmap(String url) {
                            return cache.get(url);
                        }
                    });

            // Foto de usuario
            if (urlFoto != null) {
                NetworkImageView foto = (NetworkImageView) findViewById(R.id.fotoPerfil);
                foto.setImageUrl(urlFoto.toString(), lectorImagenes);
            }

            nombre = findViewById(R.id.inputNombre);
            nombre.setText(nombreUsuario);

            correo = findViewById(R.id.inputEmail);
            correo.setText(correoUsuario);
        }

        else{
            //si iniciamos sesión con correo electrónico:
            setContentView(R.layout.perfil_usuario);

            nombreApellidos = findViewById(R.id.inputNombreApellidos);
            nombreApellidos.setText(nombreUsuario);

            correo = findViewById(R.id.inputEmail);
            correo.setText(correoUsuario);

            contrasenya = findViewById(R.id.inputContrasenya);


        //Boton confirmar cambios:
        Button btnConfirmarCambios = findViewById(R.id.btnConfirmarCambios);
        btnConfirmarCambios.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                pulsarConfirmarCambios(v);
            }
        });
        }


        //-------------------------------------------------------------------------------------
        // FUNCIONALIDAD BOTONES MENUS
        //-------------------------------------------------------------------------------------
        MenuManager funcionMenu = new MenuManager();

        ImageView btnPerfilUsuario = findViewById(R.id.logoUsuarioHeader);
        btnPerfilUsuario.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                funcionMenu.abrirPerfilUsuario(PerfilUsuarioActivity.this);
            }
        });

        ImageView btnPantallaPrincipal = findViewById(R.id.btnPantallaPrincipal);
        btnPantallaPrincipal.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                funcionMenu.abrirPantallaPrincipal(PerfilUsuarioActivity.this);
            }
        });

        ImageView btnAjustesDescanso = findViewById(R.id.btnAjustesDescanso);
        btnAjustesDescanso.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                funcionMenu.abrirAjustesDescanso(PerfilUsuarioActivity.this);
            }
        });

        ImageView btnPreferencias = findViewById(R.id.btnPreferencias);
        btnPreferencias.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                funcionMenu.abrirAcercaDe(PerfilUsuarioActivity.this);
            }
        });


        //funcionalidad del botón de cerrar sesión:
        Button botonCerrarSesion = findViewById(R.id.btnCerrarSesion);
        botonCerrarSesion.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                cerrarSesion(v);
            }
        });
        //-------------------------------------------------------------------------------------
        // FIN DE FUNCIONALIDAD BOTONES MENUS
        //-------------------------------------------------------------------------------------

    }//Fin onCreate()

    public void cerrarSesion(View view){

        firebaseAuth.signOut();

        //Facebook Log Out Login Manager
        LoginManager.getInstance().logOut();

        Toast.makeText(this, "Se ha cerrado la sesión", Toast.LENGTH_SHORT).show();

        //después de cerrar sesión nos dirigirá a la pantalla de pre-inicio de sesión:
        Intent i = new Intent(getApplicationContext(), PreinicioDeSesion.class);
        i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP
                | Intent.FLAG_ACTIVITY_NEW_TASK
                | Intent.FLAG_ACTIVITY_CLEAR_TASK);
        startActivity(i);
        finish();
    } //Fin cerrarSesion()

    private void modificarDatosPerfil(){

        /*

        //Toast.makeText(PerfilUsuarioActivity.this, "Modificar datos", Toast.LENGTH_SHORT).show();

        correo = findViewById(R.id.inputEmail);
        String emailNuevo = correo.getText().toString();
        //Toast.makeText(PerfilUsuarioActivity.this, emailNuevo, Toast.LENGTH_SHORT).show();

        UserProfileChangeRequest perfil = new UserProfileChangeRequest.Builder()
                //.setDisplayName(nombreNuevo)
                //.setPhotoUri(Uri.parse("https://www.ejemplo.com/usuario/foto.jpg"))
                .build();


        firebaseUser.updateProfile(perfil).addOnCompleteListener(new OnCompleteListener<Void>() {
            @Override
            public void onComplete(@NonNull Task<Void> task) {
                if (!task.isSuccessful()) {
                    Log.e("Perfil", "Acción incorrecta");
                }
            }
        });
        firebaseUser.updateEmail(emailNuevo).addOnCompleteListener(new OnCompleteListener<Void>() {
            @Override
            public void onComplete(@NonNull Task<Void> task) {
                if (!task.isSuccessful()) {
                    Log.e("E-mail", "Acción incorrecta");
                }
            }
        });

        /*
        firebaseUser.updatePassword(passNueva).addOnCompleteListener(new OnCompleteListener<Void>() {
            @Override
            public void onComplete(@NonNull Task<Void> task) {
                if (!task.isSuccessful()) {
                    Log.e("Contraseña", "Acción incorrecta");
                }
            }
        });

         */

    }

    private void pulsarConfirmarCambios(View view){
        //Recogemos los datos introducidos por el usuario:
        String nombreNuevo = nombreApellidos.getText().toString();
        String emailNuevo = correo.getText().toString();

        //si no se ha cambiado el e-mail original, pero sí el nombre, cambiamos el nombre directamente.
        if(!nombreNuevo.equals(nombreUsuario) && emailNuevo.equals(correoUsuario)){
            actualizarNombreUsuario(nombreNuevo, firebaseUser);

        //si se ha cambiado el e-mail, lanzar un popUp de confirmación:
        } else if(!emailNuevo.equals(correoUsuario)){
            Intent intent = new Intent(this, PopUpComprobarDatosActivity.class);
            intent.putExtra("email", emailNuevo);
            intent.putExtra("nombre", nombreNuevo);
            activityResultLauncher.launch(intent);
        }
    }

    private void actualizarNombreUsuario(String nombre, FirebaseUser usuario){

        UserProfileChangeRequest profileUpdates = new UserProfileChangeRequest.Builder()
                .setDisplayName(nombre)
                .build();

        usuario.updateProfile(profileUpdates)
                .addOnCompleteListener(new OnCompleteListener<Void>() {
                    @Override
                    public void onComplete(@NonNull Task<Void> task) {
                        if (task.isSuccessful()) {
                            Log.d("nombreUsuario", "¡Nombre de usuario actualizado!");
                        }
                        else{
                            Log.e("nombreUsuario", "Error en actualizar nombre usuario");
                        }
                    }
                });
    }

    ActivityResultLauncher<Intent> activityResultLauncher = registerForActivityResult(
            new ActivityResultContracts.StartActivityForResult(),
            new ActivityResultCallback<ActivityResult>() {
                @Override
                public void onActivityResult(ActivityResult result) {
                    if (result.getResultCode() == Activity.RESULT_OK) {
                        Intent data = result.getData();

                        Toast.makeText(PerfilUsuarioActivity.this, "Ha llegado correcto", Toast.LENGTH_SHORT).show();

                        modificarDatosPerfil();

                        }
                    }
            });
}//Class
