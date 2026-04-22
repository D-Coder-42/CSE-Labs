from django.contrib import admin
from .models import Author, Publisher, Book

@admin.register(Author)
class AuthorAdmin(admin.ModelAdmin):
    list_display = ('first_name', 'last_name', 'email')
    search_fields = ('first_name', 'last_name', 'email')

@admin.register(Publisher)
class PublisherAdmin(admin.ModelAdmin):
    list_display = ('name', 'city', 'country', 'website')
    search_fields = ('name', 'city', 'country')

@admin.register(Book)
class BookAdmin(admin.ModelAdmin):
    list_display = ('title', 'publication_date', 'publisher')
    list_filter = ('publication_date', 'publisher')
    search_fields = ('title',)
    filter_horizontal = ('authors',) 
