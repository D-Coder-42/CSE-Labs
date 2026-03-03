from django.urls import path
from . import views

urlpatterns = [
    path('', views.select_items, name='select_items'),
]
