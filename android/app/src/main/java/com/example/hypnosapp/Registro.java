package com.example.hypnosapp;

import android.os.Bundle;
import android.view.View;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.TextView;
import android.app.DatePickerDialog;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.example.hypnosapp.databinding.RegistroBinding;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;

public class Registro extends AppCompatActivity {

    public RegistroBinding binding;
    private FirebaseAuth auth = FirebaseAuth.getInstance();
    private EditText etCorreo, etContraseña, etRepContraseña, etNombreApellido, etFecha;
    private TextView tvCorreo, tvContraseña, tvRepContraseña, tvNombreApellido, tvFecha, tvRespuesta;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = RegistroBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        etCorreo = binding.edtEmailRegistro;
        etContraseña = binding.edtContraseARegistro;
        etRepContraseña = binding.edtRepetirContraseA;
        etNombreApellido = binding.edtNombreApellidos;
        etFecha = binding.edtFechaNacimiento;
        tvCorreo = binding.tvemailre;
        tvContraseña = binding.tvcontrare;
        tvRepContraseña = binding.tvcontraredos;
        tvNombreApellido = binding.tvnombre;
        tvFecha = binding.tvfecha;

        etFecha.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mostrarDatePickerDialog();
            }
        });
    }

    public void registroCorreo(View v) {

        String correo = etCorreo.getText().toString();;
        String contraseña = etContraseña.getText().toString();;
        String nombreCompleto = etNombreApellido.getText().toString();
        String fechaNacimiento = etFecha.getText().toString();

        if (AuthHelper.verificaCredenciales(etCorreo, etContraseña, tvCorreo, tvContraseña) &&
                AuthHelper.verificaCamposRegistro(etNombreApellido, etFecha, tvNombreApellido, tvFecha) &&
                AuthHelper.verificaContraseña(etContraseña, etRepContraseña, tvRepContraseña)) {
            AuthHelper.registrarUsuario(auth, correo, contraseña, nombreCompleto, fechaNacimiento, this, new OnCompleteListener<AuthResult>() {
                @Override
                public void onComplete(@NonNull Task<AuthResult> task) {
                    AuthHelper.manejoRespuestaFirebase(task, tvRespuesta, Registro.this, "com.example.hypnosapp.InicioDeSesion");
                }
            });
        }
    }

    private void mostrarDatePickerDialog() {
        DatePickerDialog datePickerDialog = new DatePickerDialog(this, new DatePickerDialog.OnDateSetListener() {
            @Override
            public void onDateSet(DatePicker view, int year, int month, int dayOfMonth) {
                String fechaSeleccionada = dayOfMonth + "/" + (month + 1) + "/" + year;
                etFecha.setText(fechaSeleccionada);
            }
        }, 2023, 9, 27); // Establece la fecha inicial aquí (por ejemplo, 1 de enero de 1990)

        datePickerDialog.getDatePicker().setMaxDate(System.currentTimeMillis()); // Opcional: establece una fecha máxima (hasta la fecha actual)
        datePickerDialog.show();
    }
}