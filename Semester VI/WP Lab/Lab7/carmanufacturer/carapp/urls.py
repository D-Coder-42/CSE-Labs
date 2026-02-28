# carapp/urls.py
from django.urls import path
from . import views

urlpatterns = [
    path('', views.input, name='car_input'),
]