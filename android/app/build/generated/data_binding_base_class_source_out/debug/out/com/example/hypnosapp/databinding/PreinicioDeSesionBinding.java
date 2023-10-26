// Generated by view binder compiler. Do not edit!
package com.example.hypnosapp.databinding;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.constraintlayout.widget.Guideline;
import androidx.viewbinding.ViewBinding;
import androidx.viewbinding.ViewBindings;
import com.example.hypnosapp.R;
import com.facebook.login.widget.LoginButton;
import java.lang.NullPointerException;
import java.lang.Override;
import java.lang.String;

public final class PreinicioDeSesionBinding implements ViewBinding {
  @NonNull
  private final ConstraintLayout rootView;

  @NonNull
  public final Button btnEmail;

  @NonNull
  public final Button btnFacebook;

  @NonNull
  public final Button btnGoogle;

  @NonNull
  public final LoginButton buttonSignInFacebook;

  @NonNull
  public final Guideline guideline;

  @NonNull
  public final ImageView imageView;

  @NonNull
  public final ImageView imageView2;

  @NonNull
  public final ImageView imageView7;

  @NonNull
  public final TextView textView3;

  @NonNull
  public final TextView textView4;

  @NonNull
  public final TextView txt;

  @NonNull
  public final TextView txtRegistrate;

  private PreinicioDeSesionBinding(@NonNull ConstraintLayout rootView, @NonNull Button btnEmail,
      @NonNull Button btnFacebook, @NonNull Button btnGoogle,
      @NonNull LoginButton buttonSignInFacebook, @NonNull Guideline guideline,
      @NonNull ImageView imageView, @NonNull ImageView imageView2, @NonNull ImageView imageView7,
      @NonNull TextView textView3, @NonNull TextView textView4, @NonNull TextView txt,
      @NonNull TextView txtRegistrate) {
    this.rootView = rootView;
    this.btnEmail = btnEmail;
    this.btnFacebook = btnFacebook;
    this.btnGoogle = btnGoogle;
    this.buttonSignInFacebook = buttonSignInFacebook;
    this.guideline = guideline;
    this.imageView = imageView;
    this.imageView2 = imageView2;
    this.imageView7 = imageView7;
    this.textView3 = textView3;
    this.textView4 = textView4;
    this.txt = txt;
    this.txtRegistrate = txtRegistrate;
  }

  @Override
  @NonNull
  public ConstraintLayout getRoot() {
    return rootView;
  }

  @NonNull
  public static PreinicioDeSesionBinding inflate(@NonNull LayoutInflater inflater) {
    return inflate(inflater, null, false);
  }

  @NonNull
  public static PreinicioDeSesionBinding inflate(@NonNull LayoutInflater inflater,
      @Nullable ViewGroup parent, boolean attachToParent) {
    View root = inflater.inflate(R.layout.preinicio_de_sesion, parent, false);
    if (attachToParent) {
      parent.addView(root);
    }
    return bind(root);
  }

  @NonNull
  public static PreinicioDeSesionBinding bind(@NonNull View rootView) {
    // The body of this method is generated in a way you would not otherwise write.
    // This is done to optimize the compiled bytecode for size and performance.
    int id;
    missingId: {
      id = R.id.btnEmail;
      Button btnEmail = ViewBindings.findChildViewById(rootView, id);
      if (btnEmail == null) {
        break missingId;
      }

      id = R.id.btnFacebook;
      Button btnFacebook = ViewBindings.findChildViewById(rootView, id);
      if (btnFacebook == null) {
        break missingId;
      }

      id = R.id.btnGoogle;
      Button btnGoogle = ViewBindings.findChildViewById(rootView, id);
      if (btnGoogle == null) {
        break missingId;
      }

      id = R.id.button_sign_in_facebook;
      LoginButton buttonSignInFacebook = ViewBindings.findChildViewById(rootView, id);
      if (buttonSignInFacebook == null) {
        break missingId;
      }

      id = R.id.guideline;
      Guideline guideline = ViewBindings.findChildViewById(rootView, id);
      if (guideline == null) {
        break missingId;
      }

      id = R.id.imageView;
      ImageView imageView = ViewBindings.findChildViewById(rootView, id);
      if (imageView == null) {
        break missingId;
      }

      id = R.id.imageView2;
      ImageView imageView2 = ViewBindings.findChildViewById(rootView, id);
      if (imageView2 == null) {
        break missingId;
      }

      id = R.id.imageView7;
      ImageView imageView7 = ViewBindings.findChildViewById(rootView, id);
      if (imageView7 == null) {
        break missingId;
      }

      id = R.id.textView3;
      TextView textView3 = ViewBindings.findChildViewById(rootView, id);
      if (textView3 == null) {
        break missingId;
      }

      id = R.id.textView4;
      TextView textView4 = ViewBindings.findChildViewById(rootView, id);
      if (textView4 == null) {
        break missingId;
      }

      id = R.id.txt;
      TextView txt = ViewBindings.findChildViewById(rootView, id);
      if (txt == null) {
        break missingId;
      }

      id = R.id.txtRegistrate;
      TextView txtRegistrate = ViewBindings.findChildViewById(rootView, id);
      if (txtRegistrate == null) {
        break missingId;
      }

      return new PreinicioDeSesionBinding((ConstraintLayout) rootView, btnEmail, btnFacebook,
          btnGoogle, buttonSignInFacebook, guideline, imageView, imageView2, imageView7, textView3,
          textView4, txt, txtRegistrate);
    }
    String missingId = rootView.getResources().getResourceName(id);
    throw new NullPointerException("Missing required view with ID: ".concat(missingId));
  }
}
