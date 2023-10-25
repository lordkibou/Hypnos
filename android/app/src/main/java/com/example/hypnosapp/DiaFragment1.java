package com.example.hypnosapp;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import androidx.fragment.app.Fragment;

public class DiaFragment1 extends Fragment {

    public DiaFragment1() {
        // Constructor público vacío requerido
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflar el diseño del fragmento que deseas mostrar
        View view = inflater.inflate(R.layout.fragment_dia_1, container, false);
        // Aquí puedes inicializar las vistas y realizar otras operaciones necesarias
        return view;
    }
}