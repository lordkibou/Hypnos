package com.example.hypnosapp.mainpage;

import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentPagerAdapter;

import com.example.hypnosapp.mainpage.DiaFragment1;
import com.example.hypnosapp.mainpage.DiaFragment2;
import com.example.hypnosapp.mainpage.DiaFragment3;

public class TabsPaginaPrincipal extends FragmentPagerAdapter {
    public TabsPaginaPrincipal(FragmentManager fm) {
        super(fm);
    }

    @Override
    public Fragment getItem(int position) {
        switch (position) {
            case 0:
                return new DiaFragment1();
            case 1:
                return new DiaFragment2();
            case 2:
                return new DiaFragment3();
            default:
                return null;
        }
    }

    @Override
    public int getCount() {
        return 3; // Número de pestañas
    }

    @Nullable
    @Override
    public CharSequence getPageTitle(int position) {
        switch (position) {
            case 0:
                return "Anteayer";
            case 1:
                return "Ayer";
            case 2:
                return "Hoy";
            default:
                return null;
        }
    }

}


