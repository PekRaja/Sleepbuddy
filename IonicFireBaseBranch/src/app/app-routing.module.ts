import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';


const routes: Routes = [
  {
    path: '',
    redirectTo: 'login',
    pathMatch: 'full'
  },
  {
    path: 'home',
    loadChildren: './home/home.module#HomePageModule'
  },
  {
    path: 'list',
    loadChildren: './list/list.module#ListPageModule'
  },
  { path: 'login', loadChildren: './login/login.module#LoginPageModule' },
  { path: 'register-page', loadChildren: './register-page/register-page.module#RegisterPagePageModule' },
  { path: 'login-acc', loadChildren: './login-acc/login-acc.module#LoginAccPageModule' },
  { path: 'stat', loadChildren: './stat/stat.module#StatPageModule' },
  { path: 'settings', loadChildren: './settings/settings.module#SettingsPageModule' },  { path: 'bluetooth-page', loadChildren: './bluetooth-page/bluetooth-page.module#BluetoothPagePageModule' }

];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule {}
