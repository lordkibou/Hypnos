// Generated by view binder compiler. Do not edit!
package com.example.hypnosapp.databinding;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.viewbinding.ViewBinding;
import androidx.viewbinding.ViewBindings;
import com.android.volley.toolbox.NetworkImageView;
import com.example.hypnosapp.R;
import java.lang.NullPointerException;
import java.lang.Override;
import java.lang.String;

public final class PerfilUsuarioBinding implements ViewBinding {
  @NonNull
  private final ConstraintLayout rootView;

  @NonNull
  public final Button btnCerrarSesion;

  @NonNull
  public final Button btnConfirmarCambios;

  @NonNull
  public final ConstraintLayout constraintLayout;

  @NonNull
  public final ImageView editarFoto;

  @NonNull
  public final NetworkImageView fotoPerfil;

  @NonNull
  public final EditText inputContrasenya;

  @NonNull
  public final EditText inputEmail;

  @NonNull
  public final EditText inputNombre;

  @NonNull
  public final TextView labelEmail;

  @NonNull
  public final TextView labelEmail3;

  @NonNull
  public final TextView labelNombreApellidos;

  @NonNull
  public final TextView textView;

  private PerfilUsuarioBinding(@NonNull ConstraintLayout rootView, @NonNull Button btnCerrarSesion,
      @NonNull Button btnConfirmarCambios, @NonNull ConstraintLayout constraintLayout,
      @NonNull ImageView editarFoto, @NonNull NetworkImageView fotoPerfil,
      @NonNull EditText inputContrasenya, @NonNull EditText inputEmail,
      @NonNull EditText inputNombre, @NonNull TextView labelEmail, @NonNull TextView labelEmail3,
      @NonNull TextView labelNombreApellidos, @NonNull TextView textView) {
    this.rootView = rootView;
    this.btnCerrarSesion = btnCerrarSesion;
    this.btnConfirmarCambios = btnConfirmarCambios;
    this.constraintLayout = constraintLayout;
    this.editarFoto = editarFoto;
    this.fotoPerfil = fotoPerfil;
    this.inputContrasenya = inputContrasenya;
    this.inputEmail = inputEmail;
    this.inputNombre = inputNombre;
    this.labelEmail = labelEmail;
    this.labelEmail3 = labelEmail3;
    this.labelNombreApellidos = labelNombreApellidos;
    this.textView = textView;
  }

  @Override
  @NonNull
  public ConstraintLayout getRoot() {
    return rootView;
  }

  @NonNull
  public static PerfilUsuarioBinding inflate(@NonNull LayoutInflater inflater) {
    return inflate(inflater, null, false);
  }

  @NonNull
  public static PerfilUsuarioBinding inflate(@NonNull LayoutInflater inflater,
      @Nullable ViewGroup parent, boolean attachToParent) {
    View root = inflater.inflate(R.layout.perfil_usuario, parent, false);
    if (attachToParent) {
      parent.addView(root);
    }
    return bind(root);
  }

  @NonNull
  public static PerfilUsuarioBinding bind(@NonNull View rootView) {
    // The body of this method is generated in a way you would not otherwise write.
    // This is done to optimize the compiled bytecode for size and performance.
    int id;
    missingId: {
      id = R.id.btnCerrarSesion;
      Button btnCerrarSesion = ViewBindings.findChildViewById(rootView, id);
      if (btnCerrarSesion == null) {
        break missingId;
      }

      id = R.id.btnConfirmarCambios;
      Button btnConfirmarCambios = ViewBindings.findChildViewById(rootView, id);
      if (btnConfirmarCambios == null) {
        break missingId;
      }

      id = R.id.constraintLayout;
      ConstraintLayout constraintLayout = ViewBindings.findChildViewById(rootView, id);
      if (constraintLayout == null) {
        break missingId;
      }

      id = R.id.editarFoto;
      ImageView editarFoto = ViewBindings.findChildViewById(rootView, id);
      if (editarFoto == null) {
        break missingId;
      }

      id = R.id.fotoPerfil;
      NetworkImageView fotoPerfil = ViewBindings.findChildViewById(rootView, id);
      if (fotoPerfil == null) {
        break missingId;
      }

      id = R.id.inputContrasenya;
      EditText inputContrasenya = ViewBindings.findChildViewById(rootView, id);
      if (inputContrasenya == null) {
        break missingId;
      }

      id = R.id.inputEmail;
      EditText inputEmail = ViewBindings.findChildViewById(rootView, id);
      if (inputEmail == null) {
        break missingId;
      }

      id = R.id.inputNombre;
      EditText inputNombre = ViewBindings.findChildViewById(rootView, id);
      if (inputNombre == null) {
        break missingId;
      }

      id = R.id.labelEmail;
      TextView labelEmail = ViewBindings.findChildViewById(rootView, id);
      if (labelEmail == null) {
        break missingId;
      }

      id = R.id.labelEmail3;
      TextView labelEmail3 = ViewBindings.findChildViewById(rootView, id);
      if (labelEmail3 == null) {
        break missingId;
      }

      id = R.id.labelNombreApellidos;
      TextView labelNombreApellidos = ViewBindings.findChildViewById(rootView, id);
      if (labelNombreApellidos == null) {
        break missingId;
      }

      id = R.id.textView;
      TextView textView = ViewBindings.findChildViewById(rootView, id);
      if (textView == null) {
        break missingId;
      }

      return new PerfilUsuarioBinding((ConstraintLayout) rootView, btnCerrarSesion,
          btnConfirmarCambios, constraintLayout, editarFoto, fotoPerfil, inputContrasenya,
          inputEmail, inputNombre, labelEmail, labelEmail3, labelNombreApellidos, textView);
    }
    String missingId = rootView.getResources().getResourceName(id);
    throw new NullPointerException("Missing required view with ID: ".concat(missingId));
  }
}
