from django.urls import path
from . import views

urlpatterns = [
    path('add_author/', views.add_author, name='add_author'),
    path('add_publisher/', views.add_publisher, name='add_publisher'),
    path('add_book/', views.add_book, name='add_book'),
    path('books/', views.list_books, name='list_books'),
]
